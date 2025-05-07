#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>

std::vector<std::string> loadFileList(const std::string& lst_path) {
    std::vector<std::string> file_list;
    std::ifstream file(lst_path);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open list file: " << lst_path << std::endl;
        return file_list;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            file_list.push_back(line);
        }
    }
    
    return file_list;
}

void calculateMetrics(const cv::Mat& gt, const cv::Mat& pred, double& pixel_accuracy, double& iou) {
    cv::Mat gt_bin, pred_bin;
    cv::threshold(gt, gt_bin, 127, 255, cv::THRESH_BINARY);
    cv::threshold(pred, pred_bin, 127, 255, cv::THRESH_BINARY);
    
    cv::Mat gt_mask = (gt_bin > 0);
    cv::Mat pred_mask = (pred_bin > 0);
    
    cv::Mat intersection = gt_mask & pred_mask;
    cv::Mat union_mask = gt_mask | pred_mask;
    
    int tp = cv::countNonZero(intersection);
    int total_pixels = gt.rows * gt.cols;
    int union_pixels = cv::countNonZero(union_mask);
    
    int correct_pixels = tp + (total_pixels - cv::countNonZero(gt_mask | pred_mask));
    pixel_accuracy = static_cast<double>(correct_pixels) / total_pixels;
    
    iou = union_pixels > 0 ? static_cast<double>(tp) / union_pixels : 0.0;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: task04-03 <gt_list_file> <pred_list_file> <output_report>" << std::endl;
        return -1;
    }
    
    std::string gt_list_path = argv[1];
    std::string pred_list_path = argv[2];
    std::string report_path = argv[3];
    
    std::vector<std::string> gt_files = loadFileList(gt_list_path);
    std::vector<std::string> pred_files = loadFileList(pred_list_path);
    
    if (gt_files.size() != pred_files.size()) {
        std::cerr << "Number of ground truth files (" << gt_files.size() 
                  << ") doesn't match number of prediction files (" << pred_files.size() << ")" << std::endl;
        return -1;
    }
    
    std::ofstream report(report_path);
    if (!report.is_open()) {
        std::cerr << "Failed to open report file: " << report_path << std::endl;
        return -1;
    }
    
    report << "File,Pixel_Accuracy,IoU" << std::endl;
    
    double avg_pixel_accuracy = 0.0;
    double avg_iou = 0.0;
    
    for (size_t i = 0; i < gt_files.size(); i++) {
        cv::Mat gt = cv::imread(gt_files[i], cv::IMREAD_GRAYSCALE);
        cv::Mat pred = cv::imread(pred_files[i], cv::IMREAD_GRAYSCALE);
        
        if (gt.empty() || pred.empty()) {
            std::cerr << "Failed to read image pair: " << gt_files[i] << " / " << pred_files[i] << std::endl;
            continue;
        }
        
        if (gt.size() != pred.size()) {
            std::cerr << "Size mismatch for pair: " << gt_files[i] << " / " << pred_files[i] << std::endl;
            continue;
        }
        
        double pixel_accuracy, iou;
        calculateMetrics(gt, pred, pixel_accuracy, iou);
        
        std::filesystem::path file_path(gt_files[i]);
        report << file_path.filename().string() << "," << pixel_accuracy << "," << iou << std::endl;
        
        avg_pixel_accuracy += pixel_accuracy;
        avg_iou += iou;
    }
    
    if (!gt_files.empty()) {
        avg_pixel_accuracy /= gt_files.size();
        avg_iou /= gt_files.size();
        
        report << "Average,," << avg_pixel_accuracy << "," << avg_iou << std::endl;
    }
    
    report.close();
    std::cout << "Quality report saved to " << report_path << std::endl;
    
    return 0;
}
