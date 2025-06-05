#include "semcv.h"
#include <iostream>

namespace semcv {

bool runTests() {
    std::cout << "Running semcv library tests..." << std::endl;

    bool allPassed = true;

    allPassed &= testGammaCorrection();
    allPassed &= testNoiseGeneration();
    allPassed &= testAutoContrast();
    allPassed &= testBinarization();
    allPassed &= testLinearFiltering();
    allPassed &= testObjectDetection();
    allPassed &= testEdgeDetection();

    if (allPassed) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << "Some tests failed!" << std::endl;
    }

    return allPassed;
}

bool testGammaCorrection() {
    std::cout << "Testing gamma correction..." << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            testImage.at<uchar>(i, j) = (i + j) % 256;
        }
    }

    cv::Mat result = gammaCorrection(testImage, 2.2);

    std::cout << "Gamma correction test passed." << std::endl;
    return true;
}

bool testNoiseGeneration() {
    std::cout << "Testing noise generation..." << std::endl;

    cv::Mat testImage = cv::Mat::ones(100, 100, CV_8UC1) * 128;

    cv::Mat gaussianNoise = addGaussianNoise(testImage, 0, 25);
    cv::Mat saltPepperNoise = addSaltPepperNoise(testImage, 0.05, 0.05);
    cv::Mat uniformNoise = addUniformNoise(testImage, 50);

    std::cout << "Noise generation test passed." << std::endl;
    return true;
}

bool testAutoContrast() {
    std::cout << "Testing auto contrast..." << std::endl;

    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            testImage.at<uchar>(i, j) = (i + j) % 128 + 64;
        }
    }

    cv::Mat result = autoContrast(testImage, 1.0);
    cv::Mat claheResult = claheContrast(testImage);

    std::cout << "Auto contrast test passed." << std::endl;
    return true;
}

bool testBinarization() {
    std::cout << "Testing binarization..." << std::endl;

    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            testImage.at<uchar>(i, j) = (i + j) % 256;
        }
    }

    cv::Mat globalResult = globalThreshold(testImage, 128);
    cv::Mat otsuResult = otsuThreshold(testImage);
    cv::Mat adaptiveResult = adaptiveThreshold(testImage);

    std::cout << "Binarization test passed." << std::endl;
    return true;
}

bool testLinearFiltering() {
    std::cout << "Testing linear filtering..." << std::endl;

    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            testImage.at<uchar>(i, j) = (i + j) % 256;
        }
    }

    cv::Mat gaussianResult = gaussianFilter(testImage);
    cv::Mat boxResult = boxFilter(testImage);
    cv::Mat medianResult = medianFilter(testImage);

    std::cout << "Linear filtering test passed." << std::endl;
    return true;
}

bool testObjectDetection() {
    std::cout << "Testing object detection..." << std::endl;

    cv::Mat testImage = cv::Mat::zeros(200, 200, CV_8UC1);
    cv::Mat templateImg = cv::Mat::ones(50, 50, CV_8UC1) * 255;

    // Place template in image
    templateImg.copyTo(testImage(cv::Rect(75, 75, 50, 50)));

    std::vector<cv::Point2f> keypoints = detectKeypoints(testImage);

    std::cout << "Object detection test passed." << std::endl;
    return true;
}

bool testEdgeDetection() {
    std::cout << "Testing edge detection..." << std::endl;

    cv::Mat testImage = cv::Mat::zeros(100, 100, CV_8UC1);
    cv::rectangle(testImage, cv::Point(25, 25), cv::Point(75, 75), cv::Scalar(255), -1);

    cv::Mat sobelResult = sobelEdgeDetection(testImage);
    cv::Mat cannyResult = cannyEdgeDetection(testImage, 50, 150);
    cv::Mat laplacianResult = laplacianEdgeDetection(testImage);

    std::cout << "Edge detection test passed." << std::endl;
    return true;
}

} // namespace semcv
