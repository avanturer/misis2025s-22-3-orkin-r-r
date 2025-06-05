#include "lab7_edge_detection.h"
#include "semcv.h"
#include <iostream>

namespace lab7 {

// Implementation of functions will be here

void runLab7Demo() {
    std::cout << "=== Lab 7: Edge Detection Algorithms ===" << std::endl;

    // Create test image
    cv::Mat testImage = cv::Mat::zeros(400, 600, CV_8UC3);

    // Add some patterns for testing
    cv::rectangle(testImage, cv::Point(50, 50), cv::Point(200, 200), cv::Scalar(100, 100, 100), -1);
    cv::circle(testImage, cv::Point(400, 150), 80, cv::Scalar(200, 200, 200), -1);

    cv::imshow("Original Test Image", testImage);

    std::cout << "Lab 7 demonstration completed." << std::endl;
    cv::waitKey(0);
    cv::destroyAllWindows();
}

} // namespace lab7
