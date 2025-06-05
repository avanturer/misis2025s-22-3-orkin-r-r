/**
 * @file main.cpp
 * @brief Entry point for the Shadow Segmentation application
 * 
 * @author Created for course project
 * @date June 2025
 */

// main.cpp
#include <opencv2/opencv.hpp>
#include "ShadowDetector.h" // Ваш класс для обработки

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <image_path>\n";
        return -1;
    }

    cv::Mat input = cv::imread(argv[1]);
    if (input.empty()) {
        std::cerr << "Error loading image!\n";
        return -1;
    }

    ShadowDetector detector;
    cv::Mat result = detector.processImage(input);

    cv::imshow("Original", input);
    cv::imshow("Shadow Segmentation", result);
    cv::waitKey(0);
    
    return 0;
}

