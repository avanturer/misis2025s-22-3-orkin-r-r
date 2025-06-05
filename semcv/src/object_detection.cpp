#include "semcv.h"

namespace semcv {

std::vector<cv::Rect> detectObjects(const cv::Mat& image, const cv::Mat& templateImg, double threshold) {
    cv::Mat result;
    cv::matchTemplate(image, templateImg, result, cv::TM_CCOEFF_NORMED);

    std::vector<cv::Rect> detections;
    cv::Mat mask;
    cv::threshold(result, mask, threshold, 1.0, cv::THRESH_BINARY);

    std::vector<cv::Point2f> locations;
    cv::findNonZero(mask, locations);

    for (const auto& loc : locations) {
        cv::Rect rect(loc.x, loc.y, templateImg.cols, templateImg.rows);
        detections.push_back(rect);
    }

    return detections;
}

cv::Mat templateMatching(const cv::Mat& image, const cv::Mat& templateImg, int method) {
    cv::Mat result;
    cv::matchTemplate(image, templateImg, result, method);
    return result;
}

std::vector<cv::Point2f> detectKeypoints(const cv::Mat& image, int maxCorners, double qualityLevel, double minDistance) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack(gray, corners, maxCorners, qualityLevel, minDistance);
    return corners;
}

} // namespace semcv
