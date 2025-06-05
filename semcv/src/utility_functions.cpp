#include "semcv.h"

namespace semcv {

cv::Mat loadImage(const std::string& filename, int flags) {
    return cv::imread(filename, flags);
}

bool saveImage(const std::string& filename, const cv::Mat& image) {
    return cv::imwrite(filename, image);
}

void showImage(const std::string& windowName, const cv::Mat& image, int delay) {
    cv::imshow(windowName, image);
    if (delay == 0) {
        cv::waitKey(0);
        cv::destroyWindow(windowName);
    } else if (delay > 0) {
        cv::waitKey(delay);
    }
}

cv::Mat convertToGrayscale(const cv::Mat& image) {
    if (image.channels() == 1) {
        return image.clone();
    }

    cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    return gray;
}

cv::Mat convertColorSpace(const cv::Mat& image, int code) {
    cv::Mat result;
    cv::cvtColor(image, result, code);
    return result;
}

} // namespace semcv
