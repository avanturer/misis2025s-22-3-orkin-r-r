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

void calculateEdgeMetrics(const cv::Mat& gt, const cv::Mat& pred, double& precision, double& recall, double& f1) {
    cv::Mat gt_bin, pred_bin;
    cv::threshold(gt, gt_bin, 127, 255, cv::THRESH_BINARY);
    cv::threshold(pred, pred_bin, 127, 255, cv::THRESH_BINARY);
    
    cv::Mat gt_dilated, pred_dilated;
    cv::dilate(gt_bin, gt_dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    cv::dilate(pred_bin, pred_dilated, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    
    int tp = cv::countNonZero(gt_bin & pred_dilated);
    int gt_total = cv::countNonZero(gt_bin);
    int pred_total = cv::countNonZero(pred_bin);
    
    precision = pred_total > 0 ? static_cast<double>(tp) / pred_total : 0.0;
    recall = gt_total > 0 ? static_cast<double>(tp) / gt_total : 0.0;
    f1 = (precision + recall > 0) ? 2 * precision * recall / (precision + recall) : 0.0;
}

int main(int argc, char** argv) {
    if (argc != 4) {
        std::cout << "Usage: task07-02 <gt_list_file> <pred_list_file> <output_report>" << std::endl;
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
    
    report << "File,Precision,Recall,F1-Score" << std::endl;
    
    double avg_precision = 0.0;
    double avg_recall = 0.0;
    double avg_f1 = 0.0;
    
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
        
        double precision, recall, f1;
        calculateEdgeMetrics(gt, pred, precision, recall, f1);
        
        std::filesystem::path file_path(gt_files[i]);
        report << file_path.filename().string() << "," 
               << precision << "," << recall << "," << f1 << std::endl;
        
        avg_precision += precision;
        avg_recall += recall;
        avg_f1 += f1;
    }
    
    if (!gt_files.empty()) {
        avg_precision /= gt_files.size();
        avg_recall /= gt_files.size();
        avg_f1 /= gt_files.size();
        
        report << "Average," << avg_precision << "," << avg_recall << "," << avg_f1 << std::endl;
    }
    
    report.close();
    std::cout << "Edge detection quality report saved to " << report_path << std::endl;
    
    return 0;
}
