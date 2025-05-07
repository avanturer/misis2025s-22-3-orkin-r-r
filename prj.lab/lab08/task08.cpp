#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

cv::Mat grayWorldCorrection(const cv::Mat& img) {
    cv::Mat result = img.clone();
    
    std::vector<cv::Mat> bgr_channels;
    cv::split(result, bgr_channels);
    
    cv::Scalar means = cv::mean(result);
    double avg_b = means[0];
    double avg_g = means[1];
    double avg_r = means[2];
    
    double avg_gray = (avg_b + avg_g + avg_r) / 3.0;
    
    double scale_b = avg_gray / avg_b;
    double scale_g = avg_gray / avg_g;
    double scale_r = avg_gray / avg_r;
    
    bgr_channels[0] = bgr_channels[0] * scale_b;
    bgr_channels[1] = bgr_channels[1] * scale_g;
    bgr_channels[2] = bgr_channels[2] * scale_r;
    
    cv::merge(bgr_channels, result);
    
    return result;
}

cv::Mat advancedColorCorrection(const cv::Mat& img) {
    cv::Mat result = img.clone();
    
    std::vector<cv::Mat> bgr_channels;
    cv::split(result, bgr_channels);
    
    for (int c = 0; c < 3; c++) {
        int histSize = 256;
        float range[] = {0, 256};
        const float* histRange = {range};
        cv::Mat hist;
        cv::calcHist(&bgr_channels[c], 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
        
        float sum = 0;
        int low_value = 0, high_value = 255;
        float total = bgr_channels[c].rows * bgr_channels[c].cols;
        
        sum = 0;
        for (int i = 0; i < histSize; i++) {
            sum += hist.at<float>(i);
            if (sum / total >= 0.05) {
                low_value = i;
                break;
            }
        }
        
        sum = 0;
        for (int i = histSize - 1; i >= 0; i--) {
            sum += hist.at<float>(i);
            if (sum / total >= 0.05) {
                high_value = i;
                break;
            }
        }
        
        for (int y = 0; y < bgr_channels[c].rows; y++) {
            for (int x = 0; x < bgr_channels[c].cols; x++) {
                float value = bgr_channels[c].at<uchar>(y, x);
                value = 255.0 * (value - low_value) / (high_value - low_value);
                bgr_channels[c].at<uchar>(y, x) = cv::saturate_cast<uchar>(value);
            }
        }
    }
    
    cv::merge(bgr_channels, result);
    
    return result;
}

cv::Mat visualizeColorDistribution(const cv::Mat& img) {
    std::vector<cv::Mat> bgr_planes;
    cv::split(img, bgr_planes);
    
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    bool uniform = true, accumulate = false;
    
    cv::Mat b_hist, g_hist, r_hist;
    cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
    cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
    cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
    
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(255, 255, 255));
    
    cv::normalize(b_hist, b_hist, 0, hist_h, cv::NORM_MINMAX);
    cv::normalize(g_hist, g_hist, 0, hist_h, cv::NORM_MINMAX);
    cv::normalize(r_hist, r_hist, 0, hist_h, cv::NORM_MINMAX);
    
    for (int i = 1; i < histSize; i++) {
        cv::line(histImage, 
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                 cv::Point(bin_w * i, hist_h - cvRound(b_hist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2);
        cv::line(histImage, 
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                 cv::Point(bin_w * i, hist_h - cvRound(g_hist.at<float>(i))),
                 cv::Scalar(0, 255, 0), 2);
        cv::line(histImage, 
                 cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                 cv::Point(bin_w * i, hist_h - cvRound(r_hist.at<float>(i))),
                 cv::Scalar(0, 0, 255), 2);
    }
    
    return histImage;
}

double evaluateColorCorrection(const cv::Mat& original, const cv::Mat& corrected) {
    cv::Mat lab_original, lab_corrected;
    cv::cvtColor(original, lab_original, cv::COLOR_BGR2Lab);
    cv::cvtColor(corrected, lab_corrected, cv::COLOR_BGR2Lab);
    
    cv::Scalar mean_original, stddev_original, mean_corrected, stddev_corrected;
    cv::meanStdDev(lab_original, mean_original, stddev_original);
    cv::meanStdDev(lab_corrected, mean_corrected, stddev_corrected);
    
    double contrast_original = (stddev_original[0] + stddev_original[1] + stddev_original[2]) / 3.0;
    double contrast_corrected = (stddev_corrected[0] + stddev_corrected[1] + stddev_corrected[2]) / 3.0;
    
    double contrast_improvement = contrast_corrected / std::max(1.0, contrast_original);
    
    double color_shift = 0;
    for (int i = 1; i < 3; i++) { 
        color_shift += std::pow(mean_corrected[i] - mean_original[i], 2);
    }
    color_shift = std::sqrt(color_shift) / 128.0;  
    
    double score = contrast_improvement / (1 + color_shift);
    
    return score;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: task08 <command> [args...]" << std::endl;
        std::cout << "Commands:" << std::endl;
        std::cout << "  gray_world <input_image> <output_image> - apply gray world correction" << std::endl;
        std::cout << "  advanced <input_image> <output_image> - apply advanced color correction" << std::endl;
        std::cout << "  visualize <input_image> <output_visualization> - visualize color distribution" << std::endl;
        std::cout << "  evaluate <original_image> <corrected_image> - evaluate color correction quality" << std::endl;
        return -1;
    }
    
    std::string command = argv[1];
    
    if (command == "gray_world" && argc == 4) {
        cv::Mat img = cv::imread(argv[2]);
        if (img.empty()) {
            std::cerr << "Failed to read image: " << argv[2] << std::endl;
            return -1;
        }
        
        cv::Mat corrected = grayWorldCorrection(img);
        cv::imwrite(argv[3], corrected);
        std::cout << "Gray world correction applied and saved to: " << argv[3] << std::endl;
    }
    else if (command == "advanced" && argc == 4) {
        cv::Mat img = cv::imread(argv[2]);
        if (img.empty()) {
            std::cerr << "Failed to read image: " << argv[2] << std::endl;
            return -1;
        }
        
        cv::Mat corrected = advancedColorCorrection(img);
        cv::imwrite(argv[3], corrected);
        std::cout << "Advanced color correction applied and saved to: " << argv[3] << std::endl;
    }
    else if (command == "visualize" && argc == 4) {
        cv::Mat img = cv::imread(argv[2]);
        if (img.empty()) {
            std::cerr << "Failed to read image: " << argv[2] << std::endl;
            return -1;
        }
        
        cv::Mat visualization = visualizeColorDistribution(img);
        cv::imwrite(argv[3], visualization);
        std::cout << "Color distribution visualization saved to: " << argv[3] << std::endl;
    }
    else if (command == "evaluate" && argc == 4) {
        cv::Mat original = cv::imread(argv[2]);
        cv::Mat corrected = cv::imread(argv[3]);
        
        if (original.empty() || corrected.empty()) {
            std::cerr << "Failed to read images" << std::endl;
            return -1;
        }
        
        if (original.size() != corrected.size()) {
            std::cerr << "Image sizes don't match" << std::endl;
            return -1;
        }
        
        double score = evaluateColorCorrection(original, corrected);
        std::cout << "Color correction quality score: " << score << std::endl;
        if (score > 1.0) {
            std::cout << "The correction improved the image quality." << std::endl;
        } else {
            std::cout << "The correction didn't improve the image quality." << std::endl;
        }
    }
    else {
        std::cout << "Invalid command or arguments." << std::endl;
        return -1;
    }
    
    return 0;
}
