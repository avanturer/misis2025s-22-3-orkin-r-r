#ifndef LAB3_AUTO_CONTRAST_H
#define LAB3_AUTO_CONTRAST_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab3 {

    /**
     * @brief Perform automatic contrast enhancement
     * @param image Input image
     * @param clipPercent Percentage of pixels to clip from histogram
     * @return Contrast enhanced image
     */
    cv::Mat autoContrastEnhancement(const cv::Mat& image, double clipPercent = 1.0);

    /**
     * @brief Apply histogram equalization
     * @param image Input image
     * @return Histogram equalized image
     */
    cv::Mat histogramEqualization(const cv::Mat& image);

    /**
     * @brief Apply CLAHE (Contrast Limited Adaptive Histogram Equalization)
     * @param image Input image
     * @param clipLimit Clip limit for CLAHE
     * @param tileGridSize Size of grid for tiles
     * @return CLAHE enhanced image
     */
    cv::Mat claheEnhancement(const cv::Mat& image, double clipLimit = 2.0, cv::Size tileGridSize = cv::Size(8,8));

    /**
     * @brief Stretch histogram to full range
     * @param image Input image
     * @return Histogram stretched image
     */
    cv::Mat histogramStretching(const cv::Mat& image);

    /**
     * @brief Compare different contrast enhancement methods
     * @param image Input image
     * @return Vector of enhanced images using different methods
     */
    std::vector<cv::Mat> compareContrastMethods(const cv::Mat& image);

    /**
     * @brief Run all lab3 demonstrations
     */
    void runLab3Demo();

} // namespace lab3

#endif // LAB3_AUTO_CONTRAST_H
