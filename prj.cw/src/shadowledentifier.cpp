#include "shadowledentifier.h"
#include <iostream>

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

cv::Mat ShadowLedentifier::processImage(const cv::Mat& input) {
    if (input.empty()) {
        std::cerr << "Ошибка: пустое изображение!" << std::endl;
        return cv::Mat();
    }
    
    // Конвертируем в HSV и LAB
    cv::Mat hsv, lab;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(input, lab, cv::COLOR_BGR2Lab);
    
    // Создаем маски для каждого цветового пространства
    cv::Mat hsv_mask = createHSVMask(hsv);
    cv::Mat lab_mask = createLABMask(lab);
    
    // Используем OR вместо AND для объединения масок
    cv::Mat combined_mask;
    cv::bitwise_or(hsv_mask, lab_mask, combined_mask);
    
    // Применяем морфологические операции
    cv::Mat morphed_mask = applyMorphology(combined_mask);
    
    // Фильтруем мелкие контуры
    cv::Mat filtered_mask = filterContours(morphed_mask);
    
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
