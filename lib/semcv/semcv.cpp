#include "semcv.hpp"
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <cmath>

std::string strid_from_mat(const cv::Mat& img, const int n) {
    std::stringstream ss;
    ss << std::setw(n) << std::setfill('0') << img.cols << "x"
       << std::setw(n) << std::setfill('0') << img.rows << "."
       << img.channels() << ".";
    
    std::string type;
    switch(img.depth()) {
        case CV_8U: type = "uint08"; break;
        case CV_8S: type = "sint08"; break;
        case CV_16U: type = "uint16"; break;
        case CV_16S: type = "sint16"; break;
        case CV_32S: type = "sint32"; break;
        case CV_32F: type = "real32"; break;
        case CV_64F: type = "real64"; break;
        default: type = "unknown"; break;
    }
    
    ss << type;
    return ss.str();
}

std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst) {
    std::vector<std::filesystem::path> list_of_path;
    std::ifstream file(path_lst);
    std::string line;
    
    if (!file.is_open()) {
        return list_of_path;
    }
    
    std::filesystem::path parent_dir = path_lst.parent_path();
    
    while (std::getline(file, line)) {
        if (!line.empty()) {
            std::filesystem::path file_path = parent_dir / line;
            list_of_path.push_back(file_path);
        }
    }
    
    return list_of_path;
}

cv::Mat gen_stripes(int width, int height, int stripe_width) {
    cv::Mat img(height, width, CV_8UC1);
    
    for (int x = 0; x < width; x++) {
        int stripe_value = (x / stripe_width) % 256;
        for (int y = 0; y < height; y++) {
            img.at<uchar>(y, x) = static_cast<uchar>(stripe_value);
        }
    }
    
    return img;
}

cv::Mat gamma_correction(const cv::Mat& img, double gamma) {
    cv::Mat result = img.clone();
    
    cv::Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = cv::saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
    
    cv::LUT(img, lut, result);
    
    return result;
}

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2) {
    const int img_size = 256;
    const int square_size = 209;
    const int circle_radius = 83;
    
    cv::Mat tgtimg(img_size, img_size, CV_8UC1, cv::Scalar(lev0));
    
    int square_start = (img_size - square_size) / 2;
    cv::Rect square_rect(square_start, square_start, square_size, square_size);
    
    cv::rectangle(tgtimg, square_rect, cv::Scalar(lev1), -1);
    
    cv::Point center(img_size / 2, img_size / 2);
    cv::circle(tgtimg, center, circle_radius, cv::Scalar(lev2), -1);
    
    return tgtimg;
}

cv::Mat add_noise_gau(const cv::Mat& img, const int std) {
    cv::Mat noise = cv::Mat(img.size(), CV_8SC1);
    cv::randn(noise, 0, std);
    
    cv::Mat result;
    cv::add(img, noise, result, cv::noArray(), img.type());
    return result;
}

cv::Mat draw_histogram(const cv::Mat& img, const cv::Scalar& background_color) {
    cv::Mat histogram(256, 256, CV_8UC3, background_color);
    
    std::vector<int> hist(256, 0);
    for (int y = 0; y < img.rows; y++) {
        for (int x = 0; x < img.cols; x++) {
            int value = img.at<uchar>(y, x);
            hist[value]++;
        }
    }
    
    int max_value = *std::max_element(hist.begin(), hist.end());
    
    for (int i = 0; i < 256; i++) {
        int height = static_cast<int>((hist[i] * 250) / max_value);
        cv::line(histogram, 
                 cv::Point(i, 255), 
                 cv::Point(i, 255 - height), 
                 cv::Scalar(50, 50, 50), 
                 1);
    }
    
    return histogram;
}

cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white) {
    if (q_black >= q_white || q_black < 0 || q_white > 1) {
        return img.clone();
    }
    
    cv::Mat result = img.clone();
    
    if (img.channels() == 1) {
        int histSize = 256;
        float range[] = {0, 256};
        const float* histRange = {range};
        cv::Mat hist;
        cv::calcHist(&img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange);
        
        std::vector<float> cumsum(histSize);
        cumsum[0] = hist.at<float>(0);
        for (int i = 1; i < histSize; i++) {
            cumsum[i] = cumsum[i-1] + hist.at<float>(i);
        }
        
        float total = cumsum.back();
        for (auto& val : cumsum) {
            val /= total;
        }
        
        int low_idx = 0;
        while (low_idx < histSize && cumsum[low_idx] < q_black) {
            low_idx++;
        }
        
        int high_idx = histSize - 1;
        while (high_idx > 0 && cumsum[high_idx-1] > q_white) {
            high_idx--;
        }
        
        cv::Mat lut(1, 256, CV_8UC1);
        for (int i = 0; i < 256; i++) {
            if (i < low_idx) {
                lut.at<uchar>(i) = 0;
            } else if (i > high_idx) {
                lut.at<uchar>(i) = 255;
            } else {
                lut.at<uchar>(i) = cv::saturate_cast<uchar>(
                    255.0 * (i - low_idx) / (high_idx - low_idx)
                );
            }
        }
        
        cv::LUT(img, lut, result);
    } 
    else if (img.channels() == 3) {
        std::vector<cv::Mat> channels;
        cv::split(img, channels);
        
        for (auto& channel : channels) {
            channel = autocontrast(channel, q_black, q_white);
        }
        
        cv::merge(channels, result);
    }
    
    return result;
}

cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white) {
    if (img.channels() != 3) {
        return autocontrast(img, q_black, q_white);
    }
    
    cv::Mat hsv;
    cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);
    
    std::vector<cv::Mat> channels;
    cv::split(hsv, channels);
    
    channels[2] = autocontrast(channels[2], q_black, q_white);
    
    cv::Mat hsv_result;
    cv::merge(channels, hsv_result);
    
    cv::Mat result;
    cv::cvtColor(hsv_result, result, cv::COLOR_HSV2BGR);
    
    return result;
}
