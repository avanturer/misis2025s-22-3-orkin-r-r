#include "shadowledentifier.h"
#include <iostream>
#include <filesystem>

ShadowLedentifier::ShadowLedentifier() {
    // Расширенные параметры HSV для лучшего обнаружения теней
    h_min = 0;
    h_max = 179;       // Полный диапазон оттенков
    s_min = 0;
    s_max = 255;       // Полный диапазон насыщенности
    v_min = 0;
    v_max = 120;       // Уменьшили верхний порог яркости
    
    // Более мягкие параметры
    lab_l_threshold = 140;     // Увеличили порог освещенности
    min_contour_area = 100;    // Уменьшили минимальную площадь
    morph_kernel_size = 5;     // Уменьшили размер ядра
    morph_iterations = 1;      // Уменьшили количество итераций
}

cv::Mat ShadowLedentifier::processImage(const cv::Mat& input, const std::string& outputPath) {
    if (input.empty()) {
        std::cerr << "ERROR: Empty input image!" << std::endl;
        return cv::Mat();
    }
    
    // Convert to HSV and LAB color spaces
    cv::Mat hsv, lab;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(input, lab, cv::COLOR_BGR2Lab);
    
    // Create masks for each color space
    cv::Mat hsv_mask = createHSVMask(hsv);
    cv::Mat lab_mask = createLABMask(lab);
    
    // Combine masks
    cv::Mat combined_mask;
    cv::bitwise_or(hsv_mask, lab_mask, combined_mask);
    
    // Apply morphological operations
    cv::Mat morphed_mask = applyMorphology(combined_mask);
    
    // Filter small contours
    cv::Mat filtered_mask = filterContours(morphed_mask);
    
    // Save intermediate results
    if (!outputPath.empty()) {
        std::filesystem::create_directories(outputPath);
        
        std::cout << "  Saving step-by-step results to: " << outputPath << std::endl;
        cv::imwrite(outputPath + "/step1_input.jpg", input);
        cv::imwrite(outputPath + "/step2_hsv_mask.jpg", hsv_mask);
        cv::imwrite(outputPath + "/step3_lab_mask.jpg", lab_mask);
        cv::imwrite(outputPath + "/step4_combined_mask.jpg", combined_mask);
        cv::imwrite(outputPath + "/step5_morphed_mask.jpg", morphed_mask);
        cv::imwrite(outputPath + "/step6_filtered_mask.jpg", filtered_mask);
        
        cv::Mat colored_result = createColoredMask(filtered_mask, input);
        cv::imwrite(outputPath + "/step7_final_result.jpg", colored_result);
        
        std::cout << "  Step-by-step results saved successfully!" << std::endl;
    }
    
    return filtered_mask;
}

cv::Mat ShadowLedentifier::processImage(const cv::Mat& input) {
    cv::Mat filtered_mask = processImage(input, "");
    
    int total_pixels = filtered_mask.rows * filtered_mask.cols;
    int shadow_pixels = cv::countNonZero(filtered_mask);
    double shadow_percentage = (double)shadow_pixels / total_pixels * 100;
    
    std::cout << "Segmentation statistics:" << std::endl;
    std::cout << "  Total pixels: " << total_pixels << std::endl;
    std::cout << "  Shadow pixels: " << shadow_pixels << std::endl;
    std::cout << "  Shadow percentage: " << std::fixed << std::setprecision(2) << shadow_percentage << "%" << std::endl;
    
    return filtered_mask;
}



cv::Mat ShadowLedentifier::createHSVMask(const cv::Mat& hsv) {
    // Создаем маску только по яркости (V канал)
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);
    
    cv::Mat v_channel = hsv_channels[2];
    cv::Mat mask;
    
    // Тени имеют низкую яркость
    cv::threshold(v_channel, mask, v_max, 255, cv::THRESH_BINARY_INV);
    
    return mask;
}

cv::Mat ShadowLedentifier::createLABMask(const cv::Mat& lab) {
    std::vector<cv::Mat> lab_channels;
    cv::split(lab, lab_channels);
    
    cv::Mat l_channel = lab_channels[0];
    cv::Mat mask;
    
    // Тени имеют низкую освещенность (L канал)
    cv::threshold(l_channel, mask, lab_l_threshold, 255, cv::THRESH_BINARY_INV);
    
    return mask;
}

cv::Mat ShadowLedentifier::combineMasks(const cv::Mat& mask1, const cv::Mat& mask2) {
    cv::Mat combined;
    // Используем OR для более мягкого объединения
    cv::bitwise_or(mask1, mask2, combined);
    
    return combined;
}

cv::Mat ShadowLedentifier::applyMorphology(const cv::Mat& mask) {
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, 
                                              cv::Size(morph_kernel_size, morph_kernel_size));
    
    // Только операция закрытия для заполнения пробелов
    cv::morphologyEx(mask, result, cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), morph_iterations);
    
    return result;
}

cv::Mat ShadowLedentifier::filterContours(const cv::Mat& mask, int minArea) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    cv::Mat filtered = cv::Mat::zeros(mask.size(), CV_8UC1);
    
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > min_contour_area) {  // Используем переменную класса
            cv::drawContours(filtered, std::vector<std::vector<cv::Point>>{contour}, 
                           -1, cv::Scalar(255), cv::FILLED);
        }
    }
    
    return filtered;
}

cv::Mat ShadowLedentifier::createColoredMask(const cv::Mat& mask, const cv::Mat& original) {
    cv::Mat colored_result;
    original.copyTo(colored_result);
    
    // Создаем цветную маску (красный цвет для теней)
    cv::Mat colored_mask = cv::Mat::zeros(original.size(), CV_8UC3);
    colored_mask.setTo(cv::Scalar(0, 0, 255), mask); // Красный цвет
    
    // Накладываем маску с прозрачностью
    cv::addWeighted(colored_result, 0.7, colored_mask, 0.3, 0, colored_result);
    
    return colored_result;
}
