#ifndef SHADOWLEDENTIFIER_H
#define SHADOWLEDENTIFIER_H

#include <opencv2/opencv.hpp>
#include <string>
#include <filesystem>

class ShadowLedentifier {
public:
    ShadowLedentifier();
    cv::Mat processImage(const cv::Mat& input, const std::string& outputPath = "");
    cv::Mat processImage(const cv::Mat& input);
    cv::Mat createColoredMask(const cv::Mat& mask, const cv::Mat& original);
    
private:
    cv::Mat createHSVMask(const cv::Mat& hsv);
    cv::Mat createLABMask(const cv::Mat& lab);
    cv::Mat combineMasks(const cv::Mat& mask1, const cv::Mat& mask2);
    cv::Mat applyMorphology(const cv::Mat& mask);
    cv::Mat filterContours(const cv::Mat& mask, int minArea = 100);
    
    int h_min, h_max, s_min, s_max, v_min, v_max;
    int lab_l_threshold, min_contour_area;
    int morph_kernel_size, morph_iterations;
};

#endif