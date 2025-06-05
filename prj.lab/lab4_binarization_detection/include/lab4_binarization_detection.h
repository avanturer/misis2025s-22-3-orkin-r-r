#ifndef LAB4_BINARIZATION_DETECTION_H
#define LAB4_BINARIZATION_DETECTION_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab4 {

    /**
     * @brief Apply global thresholding
     * @param image Input image
     * @param threshold Threshold value
     * @return Binarized image
     */
    cv::Mat globalThresholding(const cv::Mat& image, double threshold = 128);

    /**
     * @brief Apply Otsu's thresholding
     * @param image Input image
     * @return Binarized image using Otsu's method
     */
    cv::Mat otsuThresholding(const cv::Mat& image);

    /**
     * @brief Apply adaptive thresholding
     * @param image Input image
     * @param method Adaptive method (MEAN_C or GAUSSIAN_C)
     * @param blockSize Size of neighborhood area
     * @param C Constant subtracted from mean
     * @return Adaptively binarized image
     */
    cv::Mat adaptiveThresholding(const cv::Mat& image, int method = cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                                int blockSize = 11, double C = 2);

    /**
     * @brief Detect objects using binarization
     * @param image Input image
     * @param threshold Threshold value
     * @return Vector of detected object contours
     */
    std::vector<std::vector<cv::Point>> detectObjectsByBinarization(const cv::Mat& image, double threshold = 128);

    /**
     * @brief Compare different binarization methods
     * @param image Input image
     * @return Vector of binarized images using different methods
     */
    std::vector<cv::Mat> compareBinarizationMethods(const cv::Mat& image);

    /**
     * @brief Run all lab4 demonstrations
     */
    void runLab4Demo();

} // namespace lab4

#endif // LAB4_BINARIZATION_DETECTION_H
