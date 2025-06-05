#include "semcv.h"

namespace semcv {

cv::Mat globalThreshold(const cv::Mat& image, double threshold, int maxval, int type) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::threshold(gray, result, threshold, maxval, type);
    return result;
}

cv::Mat otsuThreshold(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::threshold(gray, result, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
    return result;
}

cv::Mat adaptiveThreshold(const cv::Mat& image, int maxval, int adaptiveMethod, int type, int blockSize, double C) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::adaptiveThreshold(gray, result, maxval, adaptiveMethod, type, blockSize, C);
    return result;
}

} // namespace semcv
