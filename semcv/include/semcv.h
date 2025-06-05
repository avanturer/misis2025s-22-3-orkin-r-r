#ifndef SEMCV_H
#define SEMCV_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string>
#include <vector>

namespace semcv {

    // Gamma correction functions
    cv::Mat gammaCorrection(const cv::Mat& image, double gamma);
    cv::Mat gammaCorrection(const cv::Mat& image, double gamma, cv::Mat& lookupTable);

    // Noise generation functions
    cv::Mat addGaussianNoise(const cv::Mat& image, double mean = 0.0, double stddev = 25.0);
    cv::Mat addSaltPepperNoise(const cv::Mat& image, double saltProb = 0.05, double pepperProb = 0.05);
    cv::Mat addUniformNoise(const cv::Mat& image, double amplitude = 50.0);

    // Auto contrast functions
    cv::Mat autoContrast(const cv::Mat& image, double clipPercent = 1.0);
    cv::Mat adaptiveHistogramEqualization(const cv::Mat& image);
    cv::Mat claheContrast(const cv::Mat& image, double clipLimit = 2.0, cv::Size tileGridSize = cv::Size(8,8));

    // Binarization functions
    cv::Mat globalThreshold(const cv::Mat& image, double threshold = 128, int maxval = 255, int type = cv::THRESH_BINARY);
    cv::Mat otsuThreshold(const cv::Mat& image);
    cv::Mat adaptiveThreshold(const cv::Mat& image, int maxval = 255, int adaptiveMethod = cv::ADAPTIVE_THRESH_GAUSSIAN_C, int type = cv::THRESH_BINARY, int blockSize = 11, double C = 2);

    // Linear filtering functions
    cv::Mat gaussianFilter(const cv::Mat& image, cv::Size kernelSize = cv::Size(5,5), double sigmaX = 1.0, double sigmaY = 0);
    cv::Mat boxFilter(const cv::Mat& image, cv::Size kernelSize = cv::Size(5,5));
    cv::Mat medianFilter(const cv::Mat& image, int kernelSize = 5);
    cv::Mat bilateralFilter(const cv::Mat& image, int d = 9, double sigmaColor = 75, double sigmaSpace = 75);
    cv::Mat customLinearFilter(const cv::Mat& image, const cv::Mat& kernel);

    // Object detection functions
    std::vector<cv::Rect> detectObjects(const cv::Mat& image, const cv::Mat& templateImg, double threshold = 0.8);
    cv::Mat templateMatching(const cv::Mat& image, const cv::Mat& templateImg, int method = cv::TM_CCOEFF_NORMED);
    std::vector<cv::Point2f> detectKeypoints(const cv::Mat& image, int maxCorners = 100, double qualityLevel = 0.01, double minDistance = 10);

    // Edge detection functions
    cv::Mat sobelEdgeDetection(const cv::Mat& image, int dx = 1, int dy = 1, int kernelSize = 3);
    cv::Mat cannyEdgeDetection(const cv::Mat& image, double threshold1 = 100, double threshold2 = 200, int apertureSize = 3);
    cv::Mat laplacianEdgeDetection(const cv::Mat& image, int ddepth = CV_16S, int kernelSize = 3);
    cv::Mat scharrEdgeDetection(const cv::Mat& image, int dx = 1, int dy = 0, double scale = 1, double delta = 0);

    // Utility functions
    cv::Mat loadImage(const std::string& filename, int flags = cv::IMREAD_COLOR);
    bool saveImage(const std::string& filename, const cv::Mat& image);
    void showImage(const std::string& windowName, const cv::Mat& image, int delay = 0);
    cv::Mat convertToGrayscale(const cv::Mat& image);
    cv::Mat convertColorSpace(const cv::Mat& image, int code);

    // Test functions
    bool runTests();
    bool testGammaCorrection();
    bool testNoiseGeneration();
    bool testAutoContrast();
    bool testBinarization();
    bool testLinearFiltering();
    bool testObjectDetection();
    bool testEdgeDetection();

} // namespace semcv

#endif // SEMCV_H
