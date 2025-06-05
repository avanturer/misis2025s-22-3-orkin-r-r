#ifndef LAB6_OBJECT_DETECTION_FILTERING_H
#define LAB6_OBJECT_DETECTION_FILTERING_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab6 {

    /**
     * @brief Perform template matching
     * @param image Input image
     * @param templateImg Template image to match
     * @param method Matching method
     * @return Result of template matching
     */
    cv::Mat templateMatching(const cv::Mat& image, const cv::Mat& templateImg, 
                           int method = cv::TM_CCOEFF_NORMED);

    /**
     * @brief Detect objects using template matching
     * @param image Input image
     * @param templateImg Template image
     * @param threshold Matching threshold
     * @return Vector of detected object rectangles
     */
    std::vector<cv::Rect> detectObjectsByTemplate(const cv::Mat& image, const cv::Mat& templateImg, 
                                                 double threshold = 0.8);

    /**
     * @brief Detect corners using Harris corner detector
     * @param image Input image
     * @param threshold Corner detection threshold
     * @return Vector of detected corner points
     */
    std::vector<cv::Point2f> detectCorners(const cv::Mat& image, double threshold = 0.01);

    /**
     * @brief Detect keypoints using goodFeaturesToTrack
     * @param image Input image
     * @param maxCorners Maximum number of corners to detect
     * @param qualityLevel Quality level for corners
     * @param minDistance Minimum distance between corners
     * @return Vector of detected keypoints
     */
    std::vector<cv::Point2f> detectKeypoints(const cv::Mat& image, int maxCorners = 100, 
                                           double qualityLevel = 0.01, double minDistance = 10);

    /**
     * @brief Apply morphological operations for object detection
     * @param image Input binary image
     * @param operation Morphological operation type
     * @param kernelSize Size of structuring element
     * @return Processed image
     */
    cv::Mat morphologicalOperation(const cv::Mat& image, int operation, int kernelSize = 5);

    /**
     * @brief Compare different object detection methods
     * @param image Input image
     * @param templateImg Template for matching
     * @return Vector containing results of different detection methods
     */
    std::vector<cv::Mat> compareDetectionMethods(const cv::Mat& image, const cv::Mat& templateImg);

    /**
     * @brief Run all lab6 demonstrations
     */
    void runLab6Demo();

} // namespace lab6

#endif // LAB6_OBJECT_DETECTION_FILTERING_H
