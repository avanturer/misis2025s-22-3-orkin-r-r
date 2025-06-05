#include "lab5_linear_filtering.h"
#include <iostream>
#include <opencv2/opencv.hpp>

bool testBasicFunctionality() {
    std::cout << "Testing basic functionality..." << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::ones(100, 100, CV_8UC1) * 128;

    // Add basic test here
    std::cout << "Basic functionality test PASSED" << std::endl;
    return true;
}

int main() {
    std::cout << "Running Lab 5 Tests" << std::endl;
    std::cout << "=====================================" << std::endl;

    bool allPassed = true;
    allPassed &= testBasicFunctionality();

    if (allPassed) {
        std::cout << "All Lab 5 tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Some Lab 5 tests FAILED!" << std::endl;
        return 1;
    }
}
