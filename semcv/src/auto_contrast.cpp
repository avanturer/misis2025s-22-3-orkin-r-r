#include "semcv.h"

namespace semcv {

cv::Mat autoContrast(const cv::Mat& image, double clipPercent) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    std::vector<int> histogram(256, 0);
    for (int y = 0; y < gray.rows; y++) {
        for (int x = 0; x < gray.cols; x++) {
            histogram[gray.at<uchar>(y, x)]++;
        }
    }

    std::vector<int> cumulative(256, 0);
    cumulative[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cumulative[i] = cumulative[i-1] + histogram[i];
    }

    int total = gray.rows * gray.cols;
    int clipCount = static_cast<int>(total * clipPercent / 100.0 / 2.0);

    int minGray = 0, maxGray = 255;
    while (cumulative[minGray] < clipCount && minGray < 255) minGray++;
    while (cumulative[maxGray] >= (total - clipCount) && maxGray > 0) maxGray--;

    double alpha = 255.0 / (maxGray - minGray);
    double beta = -minGray * alpha;

    cv::Mat result;
    image.convertTo(result, -1, alpha, beta);
    return result;
}

cv::Mat adaptiveHistogramEqualization(const cv::Mat& image) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Mat result;
    cv::equalizeHist(gray, result);

    if (image.channels() == 3) {
        cv::Mat channels[3];
        cv::split(image, channels);
        cv::merge(channels, 3, result);
        cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(result, result);
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }

    return result;
}

cv::Mat claheContrast(const cv::Mat& image, double clipLimit, cv::Size tileGridSize) {
    cv::Mat gray;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->setTilesGridSize(tileGridSize);

    cv::Mat result;
    clahe->apply(gray, result);

    if (image.channels() == 3) {
        cv::cvtColor(result, result, cv::COLOR_GRAY2BGR);
    }

    return result;
}

}
