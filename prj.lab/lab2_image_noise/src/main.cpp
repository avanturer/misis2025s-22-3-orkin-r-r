#include "lab2_image_noise.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    std::cout << "Lab 2: Image Noise" << std::endl;
    std::cout << "==================" << std::endl;

    if (argc > 1) {
        // If image filename is provided as argument
        std::string filename = argv[1];
        std::cout << "Loading image: " << filename << std::endl;

        cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
        if (!image.empty()) {
            cv::imshow("Original Image", image);

            // Add different types of noise
            cv::Mat gaussianNoisy = lab2::addGaussianNoise(image, 0, 25);
            cv::Mat saltPepperNoisy = lab2::addSaltPepperNoise(image, 0.05, 0.05);
            cv::Mat uniformNoisy = lab2::addUniformNoise(image, 30);

            cv::imshow("Gaussian Noise", gaussianNoisy);
            cv::imshow("Salt & Pepper Noise", saltPepperNoisy);
            cv::imshow("Uniform Noise", uniformNoisy);

            // Test denoising methods
            std::vector<cv::Mat> denoisedResults = lab2::compareDenoisingMethods(gaussianNoisy);
            std::vector<std::string> methodNames = {
                "Gaussian Filter", "Median Filter", "Bilateral Filter", 
                "Morphological Opening", "Non-local Means"
            };

            for (size_t i = 0; i < denoisedResults.size() && i < methodNames.size(); ++i) {
                cv::imshow(methodNames[i], denoisedResults[i]);
                double psnr = lab2::calculatePSNR(image, denoisedResults[i]);
                std::cout << methodNames[i] << " PSNR: " << psnr << " dB" << std::endl;
            }

            // Analyze noise characteristics
            lab2::analyzeNoiseCharacteristics(image, gaussianNoisy);

            cv::waitKey(0);
            cv::destroyAllWindows();
        } else {
            std::cerr << "Could not load image: " << filename << std::endl;
            return -1;
        }
    } else {
        // Run demonstration with generated test image
        lab2::runLab2Demo();
    }

    return 0;
}
