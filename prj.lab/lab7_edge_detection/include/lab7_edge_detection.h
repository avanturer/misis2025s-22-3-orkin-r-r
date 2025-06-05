#ifndef LAB7_EDGE_DETECTION_H
#define LAB7_EDGE_DETECTION_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab7 {

    /**
     * @brief Apply Sobel edge detection
     * @param image Input image
     * @param dx Order of derivative in x direction
     * @param dy Order of derivative in y direction
     * @param kernelSize Size of Sobel kernel
     * @return Edge detected image
     */
    cv::Mat sobelEdgeDetection(const cv::Mat& image, int dx = 1, int dy = 1, int kernelSize = 3);

    /**
     * @brief Apply Canny edge detection
     * @param image Input image
     * @param threshold1 Lower threshold for edge linking
     * @param threshold2 Upper threshold for edge linking
     * @param apertureSize Aperture size for Sobel operator
     * @return Edge detected image
     */
    cv::Mat cannyEdgeDetection(const cv::Mat& image, double threshold1 = 100, double threshold2 = 200, 
                              int apertureSize = 3);

    /**
     * @brief Apply Laplacian edge detection
     * @param image Input image
     * @param ddepth Desired depth of destination image
     * @param kernelSize Size of Laplacian kernel
     * @return Edge detected image
     */
    cv::Mat laplacianEdgeDetection(const cv::Mat& image, int ddepth = CV_16S, int kernelSize = 3);

    /**
     * @brief Apply Scharr edge detection
     * @param image Input image
     * @param dx Order of derivative in x direction
     * @param dy Order of derivative in y direction
     * @return Edge detected image
     */
    cv::Mat scharrEdgeDetection(const cv::Mat& image, int dx = 1, int dy = 0);

    /**
     * @brief Apply Prewitt edge detection
     * @param image Input image
     * @return Edge detected image
     */
    cv::Mat prewittEdgeDetection(const cv::Mat& image);

    /**
     * @brief Compare different edge detection methods
     * @param image Input image
     * @return Vector of edge detected images using different methods
     */
    std::vector<cv::Mat> compareEdgeDetectionMethods(const cv::Mat& image);

    /**
     * @brief Detect lines using Hough transform
     * @param edges Edge detected image
     * @param rho Distance resolution in pixels
     * @param theta Angular resolution in radians
     * @param threshold Minimum vote threshold
     * @return Vector of detected lines
     */
    std::vector<cv::Vec2f> detectLines(const cv::Mat& edges, double rho = 1, double theta = CV_PI/180, 
                                     int threshold = 100);

    /**
     * @brief Detect circles using Hough transform
     * @param image Input image
     * @param dp Inverse ratio of accumulator resolution
     * @param minDist Minimum distance between centers
     * @param param1 Upper threshold for edge detection
     * @param param2 Accumulator threshold for center detection
     * @param minRadius Minimum circle radius
     * @param maxRadius Maximum circle radius
     * @return Vector of detected circles
     */
    std::vector<cv::Vec3f> detectCircles(const cv::Mat& image, double dp = 1, double minDist = 30,
                                       double param1 = 50, double param2 = 30, 
                                       int minRadius = 0, int maxRadius = 0);

    /**
     * @brief Run all lab7 demonstrations
     */
    void runLab7Demo();

} // namespace lab7

#endif // LAB7_EDGE_DETECTION_H
