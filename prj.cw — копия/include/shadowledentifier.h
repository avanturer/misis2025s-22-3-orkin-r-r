#ifndef SHADOWLEDENTIFIER_H
#define SHADOWLEDENTIFIER_H

#include <opencv2/opencv.hpp>
#include <string>

class ShadowLedentifier {
public:
    ShadowLedentifier(int v_thresh = 80, int s_thresh = 60, int morph_size = 7, int min_area = 500);
    // Основной метод обработки
    cv::Mat processImage(const cv::Mat& input, const std::string& outputPath = "");
    // Метод для создания цветного результата
    cv::Mat createColoredMask(const cv::Mat& mask, const cv::Mat& original);
private:
    int value_threshold;      // Порог яркости V для HSV
    int saturation_threshold; // Порог насыщенности S для HSV
    int morph_kernel_size;    // Размер морфологического ядра
    int min_shadow_area;      // Минимальная площадь тени
};

#endif
