#include "semcv.h"
#include <random>

namespace semcv {

cv::Mat addGaussianNoise(const cv::Mat& image, double mean, double stddev) {
    cv::Mat noise = cv::Mat::zeros(image.size(), image.type());
    cv::randn(noise, cv::Scalar(mean), cv::Scalar(stddev));

    cv::Mat result;
    cv::add(image, noise, result);
    return result;
}

cv::Mat addSaltPepperNoise(const cv::Mat& image, double saltProb, double pepperProb) {
    cv::Mat result = image.clone();
    cv::Mat noise = cv::Mat::zeros(image.size(), CV_8U);
    cv::randu(noise, 0, 255);

    // Apply salt noise (white pixels)
    cv::Mat saltMask = noise < (saltProb * 255);
    result.setTo(cv::Scalar::all(255), saltMask);

    // Apply pepper noise (black pixels)
    cv::Mat pepperMask = noise > ((1.0 - pepperProb) * 255);
    result.setTo(cv::Scalar::all(0), pepperMask);

    return result;
}

cv::Mat addUniformNoise(const cv::Mat& image, double amplitude) {
    cv::Mat noise = cv::Mat::zeros(image.size(), image.type());
    cv::randu(noise, cv::Scalar(-amplitude), cv::Scalar(amplitude));

    cv::Mat result;
    cv::add(image, noise, result);
    return result;
}

} // namespace semcv
