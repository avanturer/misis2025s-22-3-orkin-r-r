#include "shadowledentifier.h"

ShadowIdentifier::ShadowIdentifier()
    : hMin(0), hMax(30), sMin(0), sMax(60), vMin(0), vMax(80)
{
}

void ShadowIdentifier::setHsvThresholds(int hMin, int hMax, int sMin, int sMax, int vMin, int vMax)
{
    this->hMin = hMin;
    this->hMax = hMax;
    this->sMin = sMin;
    this->sMax = sMax;
    this->vMin = vMin;
    this->vMax = vMax;
}

cv::Mat ShadowIdentifier::processImage(const cv::Mat &inputImage)
{
    if (inputImage.empty()) {
        return cv::Mat();
    }

    // Convert to HSV color space
    cv::Mat hsvImage;
    cv::cvtColor(inputImage, hsvImage, cv::COLOR_BGR2HSV);

    // Apply HSV thresholding to detect shadows
    cv::Mat shadowMask = applyHsvThreshold(hsvImage);

    // Post-process the mask to remove noise and improve results
    cv::Mat processedMask = postProcessMask(shadowMask);

    // Create result image showing shadows in red overlay
    cv::Mat result;
    inputImage.copyTo(result);

    // Create red overlay for shadow regions
    cv::Mat colorMask;
    cv::cvtColor(processedMask, colorMask, cv::COLOR_GRAY2BGR);

    // Make shadow regions red
    cv::Mat redOverlay = cv::Mat::zeros(result.size(), CV_8UC3);
    redOverlay.setTo(cv::Scalar(0, 0, 255), processedMask);

    // Blend the overlay with the original image
    cv::addWeighted(result, 0.7, redOverlay, 0.3, 0, result);

    return result;
}

cv::Mat ShadowIdentifier::applyHsvThreshold(const cv::Mat &hsvImage)
{
    cv::Mat mask;

    // Handle hue wrap-around (e.g., red color at 0 and 179)
    if (hMin <= hMax) {
        cv::inRange(hsvImage, cv::Scalar(hMin, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask);
    } else {
        // Handle wrap-around case
        cv::Mat mask1, mask2;
        cv::inRange(hsvImage, cv::Scalar(0, sMin, vMin), cv::Scalar(hMax, sMax, vMax), mask1);
        cv::inRange(hsvImage, cv::Scalar(hMin, sMin, vMin), cv::Scalar(179, sMax, vMax), mask2);
        cv::bitwise_or(mask1, mask2, mask);
    }

    return mask;
}

cv::Mat ShadowIdentifier::postProcessMask(const cv::Mat &mask)
{
    cv::Mat result;

    // Apply morphological operations to clean up the mask
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

    // Opening to remove small noise
    cv::morphologyEx(mask, result, cv::MORPH_OPEN, kernel);

    // Closing to fill small gaps
    cv::morphologyEx(result, result, cv::MORPH_CLOSE, kernel);

    // Apply Gaussian blur to smooth edges
    cv::GaussianBlur(result, result, cv::Size(5, 5), 0);

    // Threshold again to ensure binary mask
    cv::threshold(result, result, 127, 255, cv::THRESH_BINARY);

    return result;
}
