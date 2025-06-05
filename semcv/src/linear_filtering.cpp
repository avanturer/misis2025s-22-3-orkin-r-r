#include "semcv.h"

namespace semcv {

cv::Mat gaussianFilter(const cv::Mat& image, cv::Size kernelSize, double sigmaX, double sigmaY) {
    if (sigmaY == 0) sigmaY = sigmaX;

    cv::Mat result;
    cv::GaussianBlur(image, result, kernelSize, sigmaX, sigmaY);
    return result;
}

cv::Mat boxFilter(const cv::Mat& image, cv::Size kernelSize) {
    cv::Mat result;
    cv::blur(image, result, kernelSize);
    return result;
}

cv::Mat medianFilter(const cv::Mat& image, int kernelSize) {
    cv::Mat result;
    cv::medianBlur(image, result, kernelSize);
    return result;
}

cv::Mat bilateralFilter(const cv::Mat& image, int d, double sigmaColor, double sigmaSpace) {
    cv::Mat result;
    cv::bilateralFilter(image, result, d, sigmaColor, sigmaSpace);
    return result;
}

cv::Mat customLinearFilter(const cv::Mat& image, const cv::Mat& kernel) {
    cv::Mat result;
    cv::filter2D(image, result, -1, kernel);
    return result;
}

} // namespace semcv
