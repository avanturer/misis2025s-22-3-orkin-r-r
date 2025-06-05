#include "lab2_image_noise.h"
#include "semcv.h"
#include <iostream>
#include <random>
#include <cmath>

namespace lab2 {

cv::Mat addGaussianNoise(const cv::Mat& image, double mean, double stddev) {
    return semcv::addGaussianNoise(image, mean, stddev);
}

cv::Mat addSaltPepperNoise(const cv::Mat& image, double saltProb, double pepperProb) {
    return semcv::addSaltPepperNoise(image, saltProb, pepperProb);
}

cv::Mat addUniformNoise(const cv::Mat& image, double amplitude) {
    return semcv::addUniformNoise(image, amplitude);
}

cv::Mat addPoissonNoise(const cv::Mat& image) {
    cv::Mat result = image.clone();

    // Convert to float for processing
    cv::Mat floatImg;
    result.convertTo(floatImg, CV_32F);

    // Apply Poisson noise (approximated using Gaussian for large lambda values)
    cv::Mat noise = cv::Mat::zeros(image.size(), CV_32F);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int y = 0; y < floatImg.rows; ++y) {
        for (int x = 0; x < floatImg.cols; ++x) {
            if (floatImg.channels() == 1) {
                float lambda = floatImg.at<float>(y, x);
                if (lambda > 0) {
                    std::poisson_distribution<int> poisson(lambda);
                    floatImg.at<float>(y, x) = poisson(gen);
                }
            } else if (floatImg.channels() == 3) {
                cv::Vec3f& pixel = floatImg.at<cv::Vec3f>(y, x);
                for (int c = 0; c < 3; ++c) {
                    float lambda = pixel[c];
                    if (lambda > 0) {
                        std::poisson_distribution<int> poisson(lambda);
                        pixel[c] = poisson(gen);
                    }
                }
            }
        }
    }

    // Convert back to original type
    floatImg.convertTo(result, image.type());
    return result;
}

cv::Mat addSpeckleNoise(const cv::Mat& image, double variance) {
    cv::Mat noise = cv::Mat::zeros(image.size(), CV_32F);
    cv::randn(noise, 0, sqrt(variance));

    cv::Mat floatImg;
    image.convertTo(floatImg, CV_32F, 1.0/255.0);

    // Speckle noise: J = I + I*n where n is zero-mean noise
    cv::Mat result = floatImg + floatImg.mul(noise);

    // Convert back to original format
    cv::Mat finalResult;
    result.convertTo(finalResult, image.type(), 255.0);
    return finalResult;
}

std::vector<cv::Mat> compareDenoisingMethods(const cv::Mat& noisyImage) {
    std::vector<cv::Mat> results;

    // 1. Gaussian blur
    cv::Mat gaussianDenoised = semcv::gaussianFilter(noisyImage, cv::Size(5, 5), 1.0);
    results.push_back(gaussianDenoised);

    // 2. Median filter
    cv::Mat medianDenoised = semcv::medianFilter(noisyImage, 5);
    results.push_back(medianDenoised);

    // 3. Bilateral filter
    cv::Mat bilateralDenoised = semcv::bilateralFilter(noisyImage, 9, 75, 75);
    results.push_back(bilateralDenoised);

    // 4. Morphological opening (for salt-pepper noise)
    cv::Mat morphDenoised;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(noisyImage, morphDenoised, cv::MORPH_OPEN, kernel);
    results.push_back(morphDenoised);

    // 5. Non-local means denoising
    cv::Mat nlmDenoised;
    if (noisyImage.channels() == 1) {
        cv::fastNlMeansDenoising(noisyImage, nlmDenoised, 3, 7, 21);
    } else {
        cv::fastNlMeansDenoisingColored(noisyImage, nlmDenoised, 3, 3, 7, 21);
    }
    results.push_back(nlmDenoised);

    return results;
}

double calculatePSNR(const cv::Mat& original, const cv::Mat& noisy) {
    cv::Mat diff;
    cv::absdiff(original, noisy, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    cv::Scalar mse = cv::mean(diff);
    double mseValue = mse[0];

    if (mseValue == 0) return std::numeric_limits<double>::infinity();

    double maxPixelValue = 255.0;
    double psnr = 10.0 * log10((maxPixelValue * maxPixelValue) / mseValue);
    return psnr;
}

double calculateMSE(const cv::Mat& original, const cv::Mat& noisy) {
    cv::Mat diff;
    cv::absdiff(original, noisy, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);

    cv::Scalar mse = cv::mean(diff);
    return mse[0];
}

void analyzeNoiseCharacteristics(const cv::Mat& original, const cv::Mat& noisy) {
    std::cout << "=== Noise Analysis ===" << std::endl;

    // Calculate noise statistics
    cv::Mat noise;
    cv::absdiff(original, noisy, noise);

    cv::Scalar noiseMean = cv::mean(noise);
    cv::Scalar noiseStddev;
    cv::meanStdDev(noise, noiseMean, noiseStddev);

    std::cout << "Noise mean: " << noiseMean[0] << std::endl;
    std::cout << "Noise std dev: " << noiseStddev[0] << std::endl;

    // Calculate quality metrics
    double psnr = calculatePSNR(original, noisy);
    double mse = calculateMSE(original, noisy);

    std::cout << "PSNR: " << psnr << " dB" << std::endl;
    std::cout << "MSE: " << mse << std::endl;

    // Calculate SNR
    cv::Scalar signalMean = cv::mean(original);
    double snr = 20.0 * log10(signalMean[0] / noiseStddev[0]);
    std::cout << "SNR: " << snr << " dB" << std::endl;
}

void runLab2Demo() {
    std::cout << "=== Lab 2: Image Noise ===" << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::zeros(400, 600, CV_8UC3);

    // Create patterns
    cv::rectangle(testImage, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(255, 255, 255), -1);
    cv::circle(testImage, cv::Point(400, 150), 80, cv::Scalar(128, 128, 128), -1);

    // Add some texture
    for (int y = 250; y < 350; ++y) {
        for (int x = 50; x < 550; ++x) {
            if ((x + y) % 20 < 10) {
                testImage.at<cv::Vec3b>(y, x) = cv::Vec3b(200, 200, 200);
            }
        }
    }

    cv::imshow("Original Test Image", testImage);

    // Test different types of noise
    std::cout << "Adding different types of noise..." << std::endl;

    cv::Mat gaussianNoisy = addGaussianNoise(testImage, 0, 25);
    cv::Mat saltPepperNoisy = addSaltPepperNoise(testImage, 0.05, 0.05);
    cv::Mat uniformNoisy = addUniformNoise(testImage, 50);
    cv::Mat poissonNoisy = addPoissonNoise(testImage);
    cv::Mat speckleNoisy = addSpeckleNoise(testImage, 0.1);

    cv::imshow("Gaussian Noise", gaussianNoisy);
    cv::imshow("Salt & Pepper Noise", saltPepperNoisy);
    cv::imshow("Uniform Noise", uniformNoisy);
    cv::imshow("Poisson Noise", poissonNoisy);
    cv::imshow("Speckle Noise", speckleNoisy);

    // Analyze noise characteristics
    analyzeNoiseCharacteristics(testImage, gaussianNoisy);

    // Test denoising methods
    std::cout << "Testing denoising methods on Gaussian noise..." << std::endl;
    std::vector<cv::Mat> denoisedResults = compareDenoisingMethods(gaussianNoisy);

    std::vector<std::string> methodNames = {
        "Gaussian Filter", "Median Filter", "Bilateral Filter", 
        "Morphological Opening", "Non-local Means"
    };

    for (size_t i = 0; i < denoisedResults.size() && i < methodNames.size(); ++i) {
        cv::imshow(methodNames[i], denoisedResults[i]);
        double psnr = calculatePSNR(testImage, denoisedResults[i]);
        std::cout << methodNames[i] << " PSNR: " << psnr << " dB" << std::endl;
    }

    // Save results
    cv::imwrite("lab2_original.png", testImage);
    cv::imwrite("lab2_gaussian_noise.png", gaussianNoisy);
    cv::imwrite("lab2_salt_pepper_noise.png", saltPepperNoisy);

    std::cout << "Press any key to continue..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    std::cout << "Lab 2 demonstration completed." << std::endl;
}

} // namespace lab2
