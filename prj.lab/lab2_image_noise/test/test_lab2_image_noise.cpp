#include "lab2_image_noise.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <cassert>

bool testGaussianNoise() {
    std::cout << "Testing Gaussian noise..." << std::endl;

    // Create uniform test image
    cv::Mat testImage = cv::Mat::ones(100, 100, CV_8UC1) * 128;

    cv::Mat noisyImage = lab2::addGaussianNoise(testImage, 0, 25);

    // Check that image size and type are preserved
    bool sizePreserved = (noisyImage.size() == testImage.size());
    bool typePreserved = (noisyImage.type() == testImage.type());

    // Check that noise was actually added (images should be different)
    bool noiseAdded = (cv::norm(testImage, noisyImage) > 0);

    // Calculate noise statistics
    cv::Mat noise;
    cv::absdiff(testImage, noisyImage, noise);
    cv::Scalar noiseMean = cv::mean(noise);
    cv::Scalar noiseStddev;
    cv::meanStdDev(noise, noiseMean, noiseStddev);

    // Check if noise characteristics are reasonable (within 50% of expected)
    bool meanOk = std::abs(noiseMean[0]) < 12.5; // Expected mean ~0
    bool stddevOk = std::abs(noiseStddev[0] - 25) < 12.5; // Expected stddev ~25

    std::cout << "Noise mean: " << noiseMean[0] << " (expected ~0)" << std::endl;
    std::cout << "Noise stddev: " << noiseStddev[0] << " (expected ~25)" << std::endl;

    if (sizePreserved && typePreserved && noiseAdded && meanOk && stddevOk) {
        std::cout << "Gaussian noise test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Gaussian noise test FAILED" << std::endl;
        return false;
    }
}

bool testSaltPepperNoise() {
    std::cout << "Testing salt and pepper noise..." << std::endl;

    cv::Mat testImage = cv::Mat::ones(100, 100, CV_8UC1) * 128;

    cv::Mat noisyImage = lab2::addSaltPepperNoise(testImage, 0.1, 0.1);

    // Count salt (255) and pepper (0) pixels
    int saltCount = 0, pepperCount = 0;
    for (int y = 0; y < noisyImage.rows; ++y) {
        for (int x = 0; x < noisyImage.cols; ++x) {
            uchar pixel = noisyImage.at<uchar>(y, x);
            if (pixel == 255) saltCount++;
            else if (pixel == 0) pepperCount++;
        }
    }

    int totalPixels = noisyImage.rows * noisyImage.cols;
    double saltRatio = (double)saltCount / totalPixels;
    double pepperRatio = (double)pepperCount / totalPixels;

    std::cout << "Salt ratio: " << saltRatio << " (expected ~0.1)" << std::endl;
    std::cout << "Pepper ratio: " << pepperRatio << " (expected ~0.1)" << std::endl;

    // Check if ratios are within reasonable bounds (±5%)
    bool saltOk = std::abs(saltRatio - 0.1) < 0.05;
    bool pepperOk = std::abs(pepperRatio - 0.1) < 0.05;

    if (saltOk && pepperOk) {
        std::cout << "Salt and pepper noise test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Salt and pepper noise test FAILED" << std::endl;
        return false;
    }
}

bool testUniformNoise() {
    std::cout << "Testing uniform noise..." << std::endl;

    cv::Mat testImage = cv::Mat::ones(100, 100, CV_8UC1) * 128;

    cv::Mat noisyImage = lab2::addUniformNoise(testImage, 50);

    // Check that noise was added
    bool noiseAdded = (cv::norm(testImage, noisyImage) > 0);

    // Calculate noise
    cv::Mat noise;
    cv::absdiff(testImage, noisyImage, noise);

    double minVal, maxVal;
    cv::minMaxLoc(noise, &minVal, &maxVal);

    std::cout << "Noise range: " << minVal << " to " << maxVal << " (expected 0 to ~50)" << std::endl;

    // Check if noise range is reasonable
    bool rangeOk = (maxVal <= 60); // Allow some tolerance

    if (noiseAdded && rangeOk) {
        std::cout << "Uniform noise test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Uniform noise test FAILED" << std::endl;
        return false;
    }
}

bool testPSNRCalculation() {
    std::cout << "Testing PSNR calculation..." << std::endl;

    cv::Mat original = cv::Mat::ones(100, 100, CV_8UC1) * 128;
    cv::Mat identical = original.clone();
    cv::Mat noisy = lab2::addGaussianNoise(original, 0, 25);

    // PSNR with identical image should be very high (infinity)
    double psnrIdentical = lab2::calculatePSNR(original, identical);

    // PSNR with noisy image should be finite and reasonable
    double psnrNoisy = lab2::calculatePSNR(original, noisy);

    std::cout << "PSNR with identical image: " << psnrIdentical << " dB" << std::endl;
    std::cout << "PSNR with noisy image: " << psnrNoisy << " dB" << std::endl;

    bool identicalOk = (psnrIdentical > 100 || std::isinf(psnrIdentical));
    bool noisyOk = (psnrNoisy > 10 && psnrNoisy < 50); // Reasonable range for this noise level

    if (identicalOk && noisyOk) {
        std::cout << "PSNR calculation test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "PSNR calculation test FAILED" << std::endl;
        return false;
    }
}

bool testDenoisingMethods() {
    std::cout << "Testing denoising methods..." << std::endl;

    cv::Mat original = cv::Mat::zeros(100, 100, CV_8UC3);
    cv::rectangle(original, cv::Point(25, 25), cv::Point(75, 75), cv::Scalar(255, 255, 255), -1);

    cv::Mat noisy = lab2::addGaussianNoise(original, 0, 25);

    std::vector<cv::Mat> denoisedResults = lab2::compareDenoisingMethods(noisy);

    // Check that we get the expected number of results
    bool correctCount = (denoisedResults.size() == 5);

    // Check that all results have the same size and type as input
    bool correctProperties = true;
    for (const auto& result : denoisedResults) {
        if (result.size() != noisy.size() || result.type() != noisy.type()) {
            correctProperties = false;
            break;
        }
    }

    // Check that denoising improved PSNR (at least for some methods)
    double originalPSNR = lab2::calculatePSNR(original, noisy);
    bool improved = false;
    for (const auto& result : denoisedResults) {
        double denoisedPSNR = lab2::calculatePSNR(original, result);
        if (denoisedPSNR > originalPSNR) {
            improved = true;
            break;
        }
    }

    std::cout << "Original PSNR: " << originalPSNR << " dB" << std::endl;

    if (correctCount && correctProperties && improved) {
        std::cout << "Denoising methods test PASSED" << std::endl;
        return true;
    } else {
        std::cout << "Denoising methods test FAILED" << std::endl;
        return false;
    }
}

int main() {
    std::cout << "Running Lab 2 Tests" << std::endl;
    std::cout << "===================" << std::endl;

    bool allPassed = true;

    allPassed &= testGaussianNoise();
    allPassed &= testSaltPepperNoise();
    allPassed &= testUniformNoise();
    allPassed &= testPSNRCalculation();
    allPassed &= testDenoisingMethods();

    std::cout << std::endl;
    if (allPassed) {
        std::cout << "All Lab 2 tests PASSED!" << std::endl;
        return 0;
    } else {
        std::cout << "Some Lab 2 tests FAILED!" << std::endl;
        return 1;
    }
}
