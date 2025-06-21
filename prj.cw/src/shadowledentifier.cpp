#include "shadowledentifier.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <filesystem>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

ShadowLedentifier::ShadowLedentifier(int v_thresh, int s_thresh, int morph_size, int min_area)
    : value_threshold(v_thresh), saturation_threshold(s_thresh), morph_kernel_size(morph_size), min_shadow_area(min_area) {}

cv::Mat ShadowLedentifier::processImage(const cv::Mat& input, const std::string& outputPath) {
    if (input.empty()) {
        std::cerr << "ERROR: Empty input image!" << std::endl;
        return cv::Mat();
    }
    cv::Mat hsv;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);
    // 1. Маска по низкой яркости (V)
    cv::Mat v_mask;
    cv::threshold(hsv_channels[2], v_mask, value_threshold, 255, cv::THRESH_BINARY_INV);
    cv::Mat v_mask_close, v_mask_open;
    // 2. Морфология (close)
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(morph_kernel_size, morph_kernel_size));
    cv::morphologyEx(v_mask, v_mask_close, cv::MORPH_CLOSE, kernel, cv::Point(-1,-1), 2);
    // 3. Морфология (open)
    cv::morphologyEx(v_mask_close, v_mask_open, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), 1);
    // 4. Фильтрация по площади
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(v_mask_open, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat filtered = cv::Mat::zeros(v_mask_open.size(), CV_8UC1);
    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > min_shadow_area) {
            cv::drawContours(filtered, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(255), cv::FILLED);
        }
    }
    // Debug output
    if (!outputPath.empty()) {
        std::error_code ec;
        std::filesystem::create_directories(outputPath, ec);
        if (ec) {
            std::cerr << "[ERROR] Failed to create directory: " << outputPath << "\n";
        }
        auto safe_write = [](const std::string& path, const cv::Mat& img) {
            if (!cv::imwrite(path, img)) {
                std::cerr << "[ERROR] Failed to write: " << path << std::endl;
            }
        };
        safe_write(outputPath + "/1_input.jpg", input);
        safe_write(outputPath + "/2_v_mask.jpg", v_mask);
        safe_write(outputPath + "/3_v_mask_close.jpg", v_mask_close);
        safe_write(outputPath + "/4_v_mask_open.jpg", v_mask_open);
        safe_write(outputPath + "/5_filtered.jpg", filtered);
        cv::Mat colored_result = createColoredMask(filtered, input);
        safe_write(outputPath + "/6_final_overlay.jpg", colored_result);
    }
    return filtered;
}

cv::Mat ShadowLedentifier::createColoredMask(const cv::Mat& mask, const cv::Mat& original) {
    cv::Mat colored_result;
    original.copyTo(colored_result);
    cv::Mat colored_mask = cv::Mat::zeros(original.size(), CV_8UC3);
    colored_mask.setTo(cv::Scalar(255, 0, 0), mask); // Синий цвет
    cv::addWeighted(colored_result, 0.7, colored_mask, 0.7, 0, colored_result);
    return colored_result;
}
