#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

// External functions from semcv
std::string strid_from_mat(const cv::Mat& img, const int n = 4);
std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst);

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: task01-01 <path_to_lst_file>" << std::endl;
        return -1;
    }
    
    std::filesystem::path lst_path = argv[1];
    std::vector<std::filesystem::path> file_paths = get_list_of_file_paths(lst_path);
    
    for (const auto& path : file_paths) {
        cv::Mat img = cv::imread(path.string());
        if (img.empty()) {
            std::cout << path.filename().string() << "\tbad, cannot open file" << std::endl;
            continue;
        }
        
        std::string filename = path.filename().string();
        
        size_t last_dot = filename.find_last_of('.');
        if (last_dot == std::string::npos) {
            std::cout << filename << "\tbad, invalid filename format" << std::endl;
            continue;
        }
        
        std::string name_without_ext = filename.substr(0, last_dot);
        
        std::string descriptor = strid_from_mat(img);
        
        if (name_without_ext == descriptor) {
            std::cout << filename << "\tgood" << std::endl;
        } else {
            std::cout << filename << "\tbad, should be " << descriptor << std::endl;
        }
    }
    
    return 0;
}
