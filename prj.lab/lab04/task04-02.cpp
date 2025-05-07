#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: task04-02 <input_image> <output_results>" << std::endl;
        return -1;
    }
    
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    
    cv::Mat img = cv::imread(input_path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Failed to read image: " << input_path << std::endl;
        return -1;
    }
    
    cv::Mat binary;
    cv::threshold(img, binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(binary, binary, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(binary, binary, cv::MORPH_CLOSE, kernel);
    
    cv::Mat labels, stats, centroids;
    int num_objects = cv::connectedComponentsWithStats(binary, labels, stats, centroids);
    
    cv::Mat result = cv::Mat::zeros(img.size(), CV_8UC1);
    
    for (int i = 1; i < num_objects; i++) {
        int area = stats.at<int>(i, cv::CC_STAT_AREA);
        
        if (area > 50) {
            int x = stats.at<int>(i, cv::CC_STAT_LEFT);
            int y = stats.at<int>(i, cv::CC_STAT_TOP);
            int width = stats.at<int>(i, cv::CC_STAT_WIDTH);
            int height = stats.at<int>(i, cv::CC_STAT_HEIGHT);
            
            cv::Mat mask = (labels == i);
            
            result.setTo(255, mask);
        }
    }
    
    cv::imwrite(output_path, result);
    std::cout << "Detection results saved to " << output_path << std::endl;
    
    return 0;
}
