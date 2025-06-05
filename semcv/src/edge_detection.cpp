#include "semcv.h"

namespace semcv {

cv::Mat sobelEdgeDetection(const cv::Mat& image, int dx, int dy, int kernelSize) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::Sobel(gray, result, CV_16S, dx, dy, kernelSize);
    cv::convertScaleAbs(result, result);
    return result;
}

cv::Mat cannyEdgeDetection(const cv::Mat& image, double threshold1, double threshold2, int apertureSize) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::Canny(gray, result, threshold1, threshold2, apertureSize);
    return result;
}

cv::Mat laplacianEdgeDetection(const cv::Mat& image, int ddepth, int kernelSize) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::Laplacian(gray, result, ddepth, kernelSize);
    cv::convertScaleAbs(result, result);
    return result;
}

cv::Mat scharrEdgeDetection(const cv::Mat& image, int dx, int dy, double scale, double delta) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::Scharr(gray, result, CV_16S, dx, dy, scale, delta);
    cv::convertScaleAbs(result, result);
    return result;
}

} // namespace semcv
