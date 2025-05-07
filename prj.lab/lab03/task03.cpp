#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white);
cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white);

int main(int argc, char** argv) {
    if (argc != 6) {
        std::cout << "Usage: task03 <type> <input_image> <q_black> <q_white> <output_image>" << std::endl;
        std::cout << "  type: naive or rgb" << std::endl;
        return -1;
    }
    
    std::string type = argv[1];
    std::string input_path = argv[2];
    double q_black = std::stod(argv[3]);
    double q_white = std::stod(argv[4]);
    std::string output_path = argv[5];
    
    if (type != "naive" && type != "rgb") {
        std::cout << "Invalid type. Use 'naive' or 'rgb'" << std::endl;
        return -1;
    }
    
    cv::Mat img = cv::imread(input_path);
    if (img.empty()) {
        std::cout << "Could not read image: " << input_path << std::endl;
        return -1;
    }
    
    cv::Mat result;
    if (type == "naive") {
        result = autocontrast(img, q_black, q_white);
    } else {
        result = autocontrast_rgb(img, q_black, q_white);
    }
    
    cv::imwrite(output_path, result);
    std::cout << "Result saved to " << output_path << std::endl;
    
    return 0;
}
