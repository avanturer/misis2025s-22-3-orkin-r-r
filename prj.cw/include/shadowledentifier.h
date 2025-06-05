/**
 * @file shadowledentifier.h
 * @brief Header file for the shadow identification functionality
 * 
 * This file defines the ShadowIdentifier class which is responsible for
 * detecting and segmenting shadows in images using HSV color thresholding.
 * 
 * @author Created for course project
 * @date June 2025
 */

#ifndef SHADOWIDENTIFIER_H
#define SHADOWIDENTIFIER_H

#include <opencv2/opencv.hpp>

/**
 * @class ShadowIdentifier
 * @brief Class for shadow detection and segmentation in images
 * 
 * This class implements shadow detection using HSV color space thresholding
 * and includes functionality for shadow mask post-processing.
 */
class ShadowIdentifier
{
public:
    /**
     * @brief Constructor with default HSV threshold values
     */
    ShadowIdentifier();

    /**
     * @brief Set the HSV threshold values for shadow detection
     * 
     * @param hMin Minimum hue value (0-179)
     * @param hMax Maximum hue value (0-179)
     * @param sMin Minimum saturation value (0-255)
     * @param sMax Maximum saturation value (0-255)
     * @param vMin Minimum value (brightness) value (0-255)
     * @param vMax Maximum value (brightness) value (0-255)
     */
    void setHsvThresholds(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax);

    /**
     * @brief Process an image to identify shadows
     * 
     * @param inputImage Input image in BGR color space
     * @return cv::Mat Result image with highlighted shadows
     */
    cv::Mat processImage(const cv::Mat &inputImage);

private:
    // HSV thresholds for shadow segmentation
    int hMin, hMax, sMin, sMax, vMin, vMax;

    /**
     * @brief Apply HSV thresholding to detect shadows
     * 
     * @param hsvImage Input image in HSV color space
     * @return cv::Mat Binary mask of detected shadows
     */
    cv::Mat applyHsvThreshold(const cv::Mat &hsvImage);

    /**
     * @brief Apply post-processing to improve the shadow mask
     * 
     * @param mask Initial shadow mask
     * @return cv::Mat Improved shadow mask
     */
    cv::Mat postProcessMask(const cv::Mat &mask);
};

#endif // SHADOWIDENTIFIER_H
