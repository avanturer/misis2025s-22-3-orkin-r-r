#include "lab1_raster_formats_gamma.h"
#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[]) {
    std::cout << "Lab 1: Raster Formats and Gamma Correction" << std::endl;
    std::cout << "==========================================" << std::endl;

    if (argc > 1) {
        // If image filename is provided as argument
        std::string filename = argv[1];
        std::cout << "Loading image: " << filename << std::endl;

        cv::Mat image = lab1::loadAndConvertFormat(filename, "png");
        if (!image.empty()) {
            cv::imshow("Loaded Image", image);

            // Apply gamma correction
            double gamma = 2.2;
            if (argc > 2) {
                gamma = std::stod(argv[2]);
            }

            cv::Mat corrected = lab1::applyGammaCorrection(image, gamma);
            cv::imshow("Gamma Corrected", corrected);

            // Analyze histograms
            lab1::analyzeHistogram(image, corrected);

            // Save in multiple formats
            std::vector<std::string> formats = {"png", "jpg", "bmp"};
            lab1::saveInMultipleFormats(corrected, "output_gamma_" + std::to_string(gamma), formats);

            cv::waitKey(0);
            cv::destroyAllWindows();
        }
    } else {
        // Run demonstration with generated test image
        lab1::runLab1Demo();
    }

    return 0;
}
