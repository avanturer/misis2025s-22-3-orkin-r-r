#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

cv::Mat detectEdges(const cv::Mat& img) {
    cv::Mat blurred;
    cv::GaussianBlur(img, blurred, cv::Size(5, 5), 1.5);
    
    cv::Mat edges;
    cv::Canny(blurred, edges, 50, 150);
    
    return edges;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: task07-01 <input_image> <output_edges>" << std::endl;
        return -1;
    }
    
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    
    cv::Mat img = cv::imread(input_path, cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "Failed to read image: " << input_path << std::endl;
        return -1;
    }
    
    cv::Mat edges = detectEdges(img);
    
    cv::imwrite(output_path, edges);
    
    std::cout << "Edge detection results saved to: " << output_path << std::endl;
    
    return 0;
}
