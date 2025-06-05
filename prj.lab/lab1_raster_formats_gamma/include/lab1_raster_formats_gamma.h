#ifndef LAB1_RASTER_FORMATS_GAMMA_H
#define LAB1_RASTER_FORMATS_GAMMA_H

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

namespace lab1 {

    /**
     * @brief Load and convert images between different formats
     * @param filename Input image filename
     * @param outputFormat Target format (e.g., "jpg", "png", "bmp")
     * @return Converted image
     */
    cv::Mat loadAndConvertFormat(const std::string& filename, const std::string& outputFormat);

    /**
     * @brief Apply gamma correction to an image
     * @param image Input image
     * @param gamma Gamma value (< 1 makes image darker, > 1 makes lighter)
     * @return Gamma corrected image
     */
    cv::Mat applyGammaCorrection(const cv::Mat& image, double gamma);

    /**
     * @brief Create gamma lookup table
     * @param gamma Gamma value
     * @return Lookup table for gamma correction
     */
    cv::Mat createGammaLUT(double gamma);

    /**
     * @brief Compare different gamma values on an image
     * @param image Input image
     * @param gammaValues Vector of gamma values to test
     * @return Vector of corrected images
     */
    std::vector<cv::Mat> compareGammaValues(const cv::Mat& image, const std::vector<double>& gammaValues);

    /**
     * @brief Analyze image histogram before and after gamma correction
     * @param original Original image
     * @param corrected Gamma corrected image
     */
    void analyzeHistogram(const cv::Mat& original, const cv::Mat& corrected);

    /**
     * @brief Save image in different raster formats
     * @param image Input image
     * @param basename Base filename without extension
     * @param formats Vector of formats to save ("jpg", "png", "bmp", "tiff")
     */
    void saveInMultipleFormats(const cv::Mat& image, const std::string& basename, 
                              const std::vector<std::string>& formats);

    /**
     * @brief Run all lab1 demonstrations
     */
    void runLab1Demo();

} // namespace lab1

#endif // LAB1_RASTER_FORMATS_GAMMA_H
