#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <cmath>

cv::Mat generateTestImage() {
    const int square_size = 127;
    const int circle_radius = 40;
    const int width = square_size * 3;
    const int height = square_size * 2;
    
    cv::Mat img(height, width, CV_8UC1, cv::Scalar(0));
    
    std::vector<int> brightness_values = {0, 127, 255};
    
    int index = 0;
    for (int y = 0; y < 2; y++) {
        for (int x = 0; x < 3; x++) {
            int square_x = x * square_size;
            int square_y = y * square_size;
            
            int circle_x = square_x + square_size / 2;
            int circle_y = square_y + square_size / 2;

            int square_brightness = brightness_values[(index / 3) % 3];
            int circle_brightness = brightness_values[index % 3];
            index++;
            
            cv::rectangle(img, cv::Rect(square_x, square_y, square_size, square_size), 
                          cv::Scalar(square_brightness), -1);
            

            cv::circle(img, cv::Point(circle_x, circle_y), circle_radius, 
                       cv::Scalar(circle_brightness), -1);
        }
    }
    
    return img;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: task05 <test_image_output> <visualization_output>" << std::endl;
        return -1;
    }
    
    std::string test_image_path = argv[1];
    std::string visualization_path = argv[2];
    
    cv::Mat test_image = generateTestImage();
    cv::imwrite(test_image_path, test_image);
    
    cv::Mat kernel1 = (cv::Mat_<float>(3, 3) << 
                       -1, 0, 1,
                       -2, 0, 2,
                       -1, 0, 1);
    
    cv::Mat kernel2 = (cv::Mat_<float>(3, 3) << 
                       -1, -2, -1,
                       0, 0, 0,
                       1, 2, 1);
    
    cv::Mat I1, I2, I3;
    cv::filter2D(test_image, I1, CV_32F, kernel1);
    cv::filter2D(test_image, I2, CV_32F, kernel2);
    
    cv::Mat I1_squared, I2_squared, sum_squared;
    cv::multiply(I1, I1, I1_squared);
    cv::multiply(I2, I2, I2_squared);
    cv::add(I1_squared, I2_squared, sum_squared);
    cv::sqrt(sum_squared, I3);
    
    cv::Mat V1, V2, V3;
    cv::normalize(I1, V1, 0, 255, cv::NORM_MINMAX);
    cv::normalize(I2, V2, 0, 255, cv::NORM_MINMAX);
    cv::normalize(I3, V3, 0, 255, cv::NORM_MINMAX);
    
    V1.convertTo(V1, CV_8UC1);
    V2.convertTo(V2, CV_8UC1);
    V3.convertTo(V3, CV_8UC1);
    
    std::vector<cv::Mat> channels = {V3, V2, V1};
    cv::Mat V4;
    cv::merge(channels, V4);
    
    cv::Mat top_row, bottom_row, visualization;
    cv::hconcat(std::vector<cv::Mat>{V1, V2}, top_row);
    cv::hconcat(std::vector<cv::Mat>{V3, V4}, bottom_row);
    cv::vconcat(std::vector<cv::Mat>{top_row, bottom_row}, visualization);
    
    cv::imwrite(visualization_path, visualization);
    
    std::cout << "Test image saved to: " << test_image_path << std::endl;
    std::cout << "Visualization saved to: " << visualization_path << std::endl;
    
    return 0;
}
