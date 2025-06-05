#include "semcv.h"
#include <cmath>

namespace semcv {

cv::Mat gammaCorrection(const cv::Mat& image, double gamma) {
    cv::Mat lookupTable(1, 256, CV_8U);
    uchar* p = lookupTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 1.0 / gamma) * 255.0);
    }

    cv::Mat result;
    cv::LUT(image, lookupTable, result);
    return result;
}

cv::Mat gammaCorrection(const cv::Mat& image, double gamma, cv::Mat& lookupTable) {
    lookupTable = cv::Mat(1, 256, CV_8U);
    uchar* p = lookupTable.ptr();

    for (int i = 0; i < 256; ++i) {
        p[i] = cv::saturate_cast<uchar>(pow(i / 255.0, 1.0 / gamma) * 255.0);
    }

    cv::Mat result;
    cv::LUT(image, lookupTable, result);
    return result;
}

} // namespace semcv
