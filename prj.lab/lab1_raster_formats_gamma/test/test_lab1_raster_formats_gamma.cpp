#include "lab1_raster_formats_gamma.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cassert>

bool testGammaCorrection() {
    std::cout << "Testing gamma correction..." << std::endl;

    // Create test image with known values
    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            testImage.at<uchar>(i, j) = (i + j) % 256;
        }
    }

    // Test gamma = 1.0 (should return identical image)
    cv::Mat result1 = lab1::applyGammaCorrection(testImage, 1.0);
    bool identical = cv::norm(testImage, result1) < 1e-6;

    // Test gamma = 2.0 (should make image darker)
    cv::Mat result2 = lab1::applyGammaCorrection(testImage, 2.0);
    double meanOriginal = cv::mean(testImage)[0];
    double meanDarker = cv::mean(result2)[0];
    bool isDarker = meanDarker < meanOriginal;

    // Test gamma = 0.5 (should make image brighter)
    cv::Mat result3 = lab1::applyGammaCorrection(testImage, 0.5);
    double meanBrighter = cv::mean(result3)[0];
    bool isBrighter = meanBrighter > meanOriginal;

    std::cout << "Original mean: " << meanOriginal << std::endl;
    std::cout << "Darker (gamma=2.0) mean: " << meanDarker << std::endl;
    std::cout << "Brighter (gamma=0.5) mean: " << meanBrighter << std::endl;

    if (identical && isDarker && isBrighter) {
        std::cout << "Gamma correction test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Gamma correction test FAILED" << std::endl;
        return false;
    }
}

bool testLookupTable() {
    std::cout << "Testing lookup table creation..." << std::endl;

    cv::Mat lut = lab1::createGammaLUT(2.0);

    // Check LUT properties
    bool correctSize = (lut.rows == 1 && lut.cols == 256);
    bool correctType = (lut.type() == CV_8U);

    // Check that LUT values are monotonic (non-decreasing)
    bool isMonotonic = true;
    for (int i = 1; i < 256; ++i) {
        if (lut.at<uchar>(0, i) < lut.at<uchar>(0, i-1)) {
            isMonotonic = false;
            break;
        }
    }

    // Check boundary values
    bool correctBoundaries = (lut.at<uchar>(0, 0) == 0) && (lut.at<uchar>(0, 255) == 255);

    if (correctSize && correctType && isMonotonic && correctBoundaries) {
        std::cout << "Lookup table test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Lookup table test FAILED" << std::endl;
        return false;
    }
}

bool testMultipleGammaValues() {
    std::cout << "Testing multiple gamma values..." << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::ones(50, 50, CV_8UC3) * 128;

    std::vector<double> gammaValues = {0.5, 1.0, 1.5, 2.0, 2.5};
    std::vector<cv::Mat> results = lab1::compareGammaValues(testImage, gammaValues);

    bool correctCount = (results.size() == gammaValues.size());

    // Check that all results have same size as input
    bool correctSizes = true;
    for (const auto& result : results) {
        if (result.size() != testImage.size() || result.type() != testImage.type()) {
            correctSizes = false;
            break;
        }
    }

    if (correctCount && correctSizes) {
        std::cout << "Multiple gamma values test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Multiple gamma values test FAILED" << std::endl;
        return false;
    }
}

bool testImageFormats() {
    std::cout << "Testing image format saving..." << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC3);
    cv::rectangle(testImage, cv::Point(25, 25), cv::Point(75, 75), cv::Scalar(255, 255, 255), -1);

    std::vector<std::string> formats = {"png", "jpg", "bmp"};

    try {
        lab1::saveInMultipleFormats(testImage, "test_format", formats);

        // Try to load saved images to verify they were saved correctly
        bool allLoaded = true;
        for (const auto& format : formats) {
            std::string filename = "test_format." + format;
            cv::Mat loaded = cv::imread(filename);
            if (loaded.empty()) {
                allLoaded = false;
                break;
            }
        }

        if (allLoaded) {
            std::cout << "Image formats test PASSED" << std::endl;
            return true;
        } else {
            std::cout << "Image formats test FAILED - Could not load saved images" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cout << "Image formats test FAILED - Exception: " << e.what() << std::endl;
        return false;
    }
}

int main() {
    std::cout << "Running Lab 1 Tests" << std::endl;
    std::cout << "===================" << std::endl;

    bool allPassed = true;

    allPassed &= testGammaCorrection();
    allPassed &= testLookupTable();
    allPassed &= testMultipleGammaValues();
    allPassed &= testImageFormats();

    std::cout << std::endl;
    if (allPassed) {
        std::cout << "All Lab 1 tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Some Lab 1 tests FAILED!" << std::endl;
        return 1;
    }
}
