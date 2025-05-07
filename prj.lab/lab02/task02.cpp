#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2);
cv::Mat add_noise_gau(const cv::Mat& img, const int std);
cv::Mat draw_histogram(const cv::Mat& img, const cv::Scalar& background_color = cv::Scalar(220, 220, 220));

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: task02 <output_path>" << std::endl;
        return -1;
    }
    
    std::vector<std::vector<int>> levels = {
        {0, 127, 255},
        {20, 127, 235},
        {55, 127, 200},
        {90, 127, 165}
    };
    
    std::vector<int> noise_stds = {3, 7, 15};
    
    std::vector<cv::Mat> test_images;
    for (const auto& level : levels) {
        test_images.push_back(gen_tgtimg00(level[0], level[1], level[2]));
    }
    
    cv::Mat original_row;
    cv::hconcat(test_images, original_row);
    
    std::vector<cv::Mat> noisy_rows;
    noisy_rows.push_back(original_row);
    
    for (int std : noise_stds) {
        std::vector<cv::Mat> noisy_images;
        for (const auto& img : test_images) {
            noisy_images.push_back(add_noise_gau(img, std));
        }
        
        cv::Mat noisy_row;
        cv::hconcat(noisy_images, noisy_row);
        noisy_rows.push_back(noisy_row);
    }
    
    cv::Mat result;
    cv::vconcat(noisy_rows, result);
    
    std::vector<cv::Mat> histogram_rows;
    for (size_t i = 0; i < noisy_rows.size(); i++) {
        std::vector<cv::Mat> histograms;
        cv::Mat row = noisy_rows[i];
        
        for (int j = 0; j < 4; j++) {
            cv::Rect roi(j * 256, 0, 256, 256);
            cv::Mat sub_img = row(roi);
            
            cv::Scalar bg_color = ((i + j) % 2 == 0) ? 
                cv::Scalar(220, 220, 220) : cv::Scalar(200, 200, 200);
            
            histograms.push_back(draw_histogram(sub_img, bg_color));
        }
        
        cv::Mat hist_row;
        cv::hconcat(histograms, hist_row);
        histogram_rows.push_back(hist_row);
    }
    
    cv::Mat histograms_result;
    cv::vconcat(histogram_rows, histograms_result);
    
    cv::Mat final_result;
    std::vector<cv::Mat> final_parts = {result, histograms_result};
    cv::vconcat(final_parts, final_result);
    
    cv::imwrite(argv[1], final_result);
    
    std::cout << "Result saved to " << argv[1] << std::endl;
    
    return 0;
}
