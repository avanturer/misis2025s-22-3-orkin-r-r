#ifndef LAB2_IMAGE_NOISE_H
#define LAB2_IMAGE_NOISE_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab2 {

    /**
     * @brief Add Gaussian noise to an image
     * @param image Input image
     * @param mean Mean of the Gaussian noise
     * @param stddev Standard deviation of the Gaussian noise
     * @return Noisy image
     */
    cv::Mat addGaussianNoise(const cv::Mat& image, double mean = 0.0, double stddev = 25.0);

    /**
     * @brief Add salt and pepper noise to an image
     * @param image Input image
     * @param saltProb Probability of salt (white) noise
     * @param pepperProb Probability of pepper (black) noise
     * @return Noisy image
     */
    cv::Mat addSaltPepperNoise(const cv::Mat& image, double saltProb = 0.05, double pepperProb = 0.05);

    /**
     * @brief Add uniform noise to an image
     * @param image Input image
     * @param amplitude Maximum amplitude of uniform noise
     * @return Noisy image
     */
    cv::Mat addUniformNoise(const cv::Mat& image, double amplitude = 50.0);

    /**
     * @brief Add Poisson noise to an image
     * @param image Input image
     * @return Noisy image
     */
    cv::Mat addPoissonNoise(const cv::Mat& image);

    /**
     * @brief Add speckle noise to an image
     * @param image Input image
     * @param variance Variance of the speckle noise
     * @return Noisy image
     */
    cv::Mat addSpeckleNoise(const cv::Mat& image, double variance = 0.1);

    /**
     * @brief Compare different denoising methods
     * @param noisyImage Noisy input image
     * @return Vector of denoised images using different methods
     */
    std::vector<cv::Mat> compareDenoisingMethods(const cv::Mat& noisyImage);

    /**
     * @brief Calculate Peak Signal-to-Noise Ratio (PSNR)
     * @param original Original clean image
     * @param noisy Noisy image
     * @return PSNR value in dB
     */
    double calculatePSNR(const cv::Mat& original, const cv::Mat& noisy);

    /**
     * @brief Calculate Mean Squared Error (MSE)
     * @param original Original clean image
     * @param noisy Noisy image
     * @return MSE value
     */
    double calculateMSE(const cv::Mat& original, const cv::Mat& noisy);

    /**
     * @brief Analyze noise characteristics in an image
     * @param original Original image
     * @param noisy Noisy image
     */
    void analyzeNoiseCharacteristics(const cv::Mat& original, const cv::Mat& noisy);

    /**
     * @brief Run all lab2 demonstrations
     */
    void runLab2Demo();

} // namespace lab2

#endif // LAB2_IMAGE_NOISE_H
