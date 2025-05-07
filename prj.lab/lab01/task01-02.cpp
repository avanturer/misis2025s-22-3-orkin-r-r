#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

cv::Mat gen_stripes(int width = 768, int height = 30, int stripe_width = 3);
cv::Mat gamma_correction(const cv::Mat& img, double gamma);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: task01-02 <output_path>" << std::endl;
        return -1;
    }
    
    cv::Mat stripes = gen_stripes();
    
    std::vector<double> gammas = {1.8, 2.0, 2.2, 2.4, 2.6};
    
    std::vector<cv::Mat> images;
    images.push_back(stripes);
    
    for (double gamma : gammas) {
        cv::Mat corrected = gamma_correction(stripes, gamma);
        images.push_back(corrected);
    }
    
    cv::Mat collage;
    cv::vconcat(images, collage);
    
    cv::imwrite(argv[1], collage);
    
    std::cout << "Collage saved to " << argv[1] << std::endl;
    
    return 0;
}
