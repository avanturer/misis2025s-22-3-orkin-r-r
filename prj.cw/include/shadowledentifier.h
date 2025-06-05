#ifndef SHADOWLEDENTIFIER_H
#define SHADOWLEDENTIFIER_H

#include <opencv2/opencv.hpp>

class ShadowLedentifier {
public:
    ShadowLedentifier();
    cv::Mat processImage(const cv::Mat& input);
    
    // Новые методы для улучшенной обработки
    cv::Mat createHSVMask(const cv::Mat& hsv);
    cv::Mat createLABMask(const cv::Mat& lab);
    cv::Mat combineMasks(const cv::Mat& mask1, const cv::Mat& mask2);
    cv::Mat filterContours(const cv::Mat& mask, int minArea = 500);
    cv::Mat applyMorphology(const cv::Mat& mask);
    
private:
    // Параметры HSV
    int h_min, h_max;
    int s_min, s_max;
    int v_min, v_max;
    
    // Новые параметры для LAB
    int lab_l_threshold;
    int min_contour_area;
    int morph_kernel_size;
    int morph_iterations;
};

#endif
