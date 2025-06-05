#include "lab1_raster_formats_gamma.h"
#include "semcv.h"
#include <iostream>
#include <opencv2/opencv.hpp>

namespace lab1 {

cv::Mat loadAndConvertFormat(const std::string& filename, const std::string& outputFormat) {
    // Load image
    cv::Mat image = cv::imread(filename, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        return cv::Mat();
    }

    std::cout << "Loaded image: " << filename << std::endl;
    std::cout << "Image size: " << image.cols << "x" << image.rows << std::endl;
    std::cout << "Channels: " << image.channels() << std::endl;
    std::cout << "Depth: " << image.depth() << std::endl;

    return image;
}

cv::Mat applyGammaCorrection(const cv::Mat& image, double gamma) {
    return semcv::gammaCorrection(image, gamma);
}

cv::Mat createGammaLUT(double gamma) {
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar* p = lookupTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 1.0 / gamma) * 255.0);
    }

    return lookupTable;
}

std::vector<cv::Mat> compareGammaValues(const cv::Mat& image, const std::vector<double>& gammaValues) {
    std::vector<cv::Mat> results;

    for (double gamma : gammaValues) {
        cv::Mat corrected = applyGammaCorrection(image, gamma);
        results.push_back(corrected);
        std::cout << "Applied gamma correction with gamma = " << gamma << std::endl;
    }

    return results;
}

void analyzeHistogram(const cv::Mat& original, const cv::Mat& corrected) {
    // Convert to grayscale for histogram analysis
    cv::Mat grayOriginal, grayCorrected;
    if (original.channels() == 3) {
        cv::cvtColor(original, grayOriginal, cv::COLOR_BGR2GRAY);
        cv::cvtColor(corrected, grayCorrected, cv::COLOR_BGR2GRAY);
    } else {
        grayOriginal = original.clone();
        grayCorrected = corrected.clone();
    }

    // Calculate histograms
    std::vector<cv::Mat> bgr_planes_orig, bgr_planes_corr;
    cv::split(grayOriginal, bgr_planes_orig);
    cv::split(grayCorrected, bgr_planes_corr);

    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};

    cv::Mat hist_orig, hist_corr;
    cv::calcHist(&grayOriginal, 1, 0, cv::Mat(), hist_orig, 1, &histSize, &histRange);
    cv::calcHist(&grayCorrected, 1, 0, cv::Mat(), hist_corr, 1, &histSize, &histRange);

    // Create histogram images
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    cv::Mat histImage_orig(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::Mat histImage_corr(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::normalize(hist_orig, hist_orig, 0, histImage_orig.rows, cv::NORM_MINMAX, -1, cv::Mat());
    cv::normalize(hist_corr, hist_corr, 0, histImage_corr.rows, cv::NORM_MINMAX, -1, cv::Mat());

    for (int i = 1; i < histSize; i++) {
        cv::line(histImage_orig,
                cv::Point(bin_w * (i - 1), hist_h - cvRound(hist_orig.at<float>(i - 1))),
                cv::Point(bin_w * i, hist_h - cvRound(hist_orig.at<float>(i))),
                cv::Scalar(255, 255, 255), 2, 8, 0);

        cv::line(histImage_corr,
                cv::Point(bin_w * (i - 1), hist_h - cvRound(hist_corr.at<float>(i - 1))),
                cv::Point(bin_w * i, hist_h - cvRound(hist_corr.at<float>(i))),
                cv::Scalar(255, 255, 255), 2, 8, 0);
    }

    cv::imshow("Original Histogram", histImage_orig);
    cv::imshow("Corrected Histogram", histImage_corr);
}

void saveInMultipleFormats(const cv::Mat& image, const std::string& basename, 
                          const std::vector<std::string>& formats) {
    for (const auto& format : formats) {
        std::string filename = basename + "." + format;
        bool success = cv::imwrite(filename, image);
        if (success) {
            std::cout << "Saved image as: " << filename << std::endl;
        } else {
            std::cerr << "Failed to save image as: " << filename << std::endl;
        }
    }
}

void runLab1Demo() {
    std::cout << "=== Lab 1: Raster Formats and Gamma Correction ===" << std::endl;

    // Create a test image if no image is available
    cv::Mat testImage = cv::Mat::zeros(400, 600, CV_8UC3);

    // Create gradient patterns
    for (int y = 0; y < testImage.rows; ++y) {
        for (int x = 0; x < testImage.cols; ++x) {
            int intensity = (x * 255) / testImage.cols;
            testImage.at<cv::Vec3b>(y, x) = cv::Vec3b(intensity, intensity, intensity);
        }
    }

    // Add some colored regions
    cv::rectangle(testImage, cv::Point(50, 50), cv::Point(150, 150), cv::Scalar(255, 0, 0), -1);
    cv::rectangle(testImage, cv::Point(200, 50), cv::Point(300, 150), cv::Scalar(0, 255, 0), -1);
    cv::rectangle(testImage, cv::Point(350, 50), cv::Point(450, 150), cv::Scalar(0, 0, 255), -1);

    std::cout << "Created test image with gradient and colored rectangles." << std::endl;

    // Show original image
    cv::imshow("Original Test Image", testImage);

    // Test different gamma values
    std::vector<double> gammaValues = {0.5, 0.8, 1.0, 1.5, 2.2, 3.0};
    std::vector<cv::Mat> correctedImages = compareGammaValues(testImage, gammaValues);

    // Display corrected images
    for (size_t i = 0; i < correctedImages.size(); ++i) {
        std::string windowName = "Gamma " + std::to_string(gammaValues[i]);
        cv::imshow(windowName, correctedImages[i]);
    }

    // Analyze histogram for gamma = 2.2 (typical monitor gamma)
    if (correctedImages.size() >= 5) {
        analyzeHistogram(testImage, correctedImages[4]); // gamma = 2.2
    }

    // Save images in different formats
    std::vector<std::string> formats = {"png", "jpg", "bmp", "tiff"};
    saveInMultipleFormats(testImage, "test_image", formats);

    // Save gamma corrected image
    if (correctedImages.size() >= 5) {
        saveInMultipleFormats(correctedImages[4], "gamma_corrected_2_2", formats);
    }

    std::cout << "Press any key to continue..." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();

    std::cout << "Lab 1 demonstration completed." << std::endl;
}

} // namespace lab1
