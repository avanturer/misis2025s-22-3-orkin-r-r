#ifndef LAB5_LINEAR_FILTERING_H
#define LAB5_LINEAR_FILTERING_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab5 {

    /**
     * @brief Apply Gaussian blur filter
     * @param image Input image
     * @param kernelSize Size of the Gaussian kernel
     * @param sigmaX Standard deviation in X direction
     * @param sigmaY Standard deviation in Y direction
     * @return Filtered image
     */
    cv::Mat gaussianBlur(const cv::Mat& image, cv::Size kernelSize = cv::Size(5,5), 
                        double sigmaX = 1.0, double sigmaY = 0);

    /**
     * @brief Apply box filter (averaging)
     * @param image Input image
     * @param kernelSize Size of the averaging kernel
     * @return Filtered image
     */
    cv::Mat boxFilter(const cv::Mat& image, cv::Size kernelSize = cv::Size(5,5));

    /**
     * @brief Apply median filter
     * @param image Input image
     * @param kernelSize Size of the median kernel (must be odd)
     * @return Filtered image
     */
    cv::Mat medianFilter(const cv::Mat& image, int kernelSize = 5);

    /**
     * @brief Apply bilateral filter
     * @param image Input image
     * @param d Diameter of pixel neighborhood
     * @param sigmaColor Filter sigma in color space
     * @param sigmaSpace Filter sigma in coordinate space
     * @return Filtered image
     */
    cv::Mat bilateralFilter(const cv::Mat& image, int d = 9, double sigmaColor = 75, double sigmaSpace = 75);

    /**
     * @brief Apply custom linear filter
     * @param image Input image
     * @param kernel Custom convolution kernel
     * @return Filtered image
     */
    cv::Mat customLinearFilter(const cv::Mat& image, const cv::Mat& kernel);

    /**
     * @brief Compare different linear filtering methods
     * @param image Input image
     * @return Vector of filtered images using different methods
     */
    std::vector<cv::Mat> compareFilteringMethods(const cv::Mat& image);

    /**
     * @brief Create various standard kernels
     * @param type Type of kernel ("gaussian", "box", "sharpen", "edge")
     * @param size Size of kernel
     * @return Kernel matrix
     */
    cv::Mat createKernel(const std::string& type, int size = 3);

    /**
     * @brief Run all lab5 demonstrations
     */
    void runLab5Demo();

} // namespace lab5

#endif // LAB5_LINEAR_FILTERING_H
