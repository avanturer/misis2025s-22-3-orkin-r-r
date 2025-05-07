#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <random>
#include <string>

struct Config {
    int grid_size_x = 4;
    int grid_size_y = 4;
    int cell_size = 256;
    int background_brightness = 100;
    double ellipse_min_size = 20;
    double ellipse_max_size = 100;
    int ellipse_min_brightness = 150;
    int ellipse_max_brightness = 250;
    double ellipse_min_angle = 0;
    double ellipse_max_angle = 180;
    double ellipse_min_ratio = 0.5;
    double ellipse_max_ratio = 2.0;
    double blur_min_sigma = 0.5;
    double blur_max_sigma = 3.0;
    int noise_amplitude = 10;
    int margin = 32;
    unsigned int seed = 42;
};

void saveConfig(const std::string& path, const Config& config) {
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving config: " << path << std::endl;
        return;
    }
    
    file << "grid_size_x=" << config.grid_size_x << std::endl;
    file << "grid_size_y=" << config.grid_size_y << std::endl;
    file << "cell_size=" << config.cell_size << std::endl;
    file << "background_brightness=" << config.background_brightness << std::endl;
    file << "ellipse_min_size=" << config.ellipse_min_size << std::endl;
    file << "ellipse_max_size=" << config.ellipse_max_size << std::endl;
    file << "ellipse_min_brightness=" << config.ellipse_min_brightness << std::endl;
    file << "ellipse_max_brightness=" << config.ellipse_max_brightness << std::endl;
    file << "ellipse_min_angle=" << config.ellipse_min_angle << std::endl;
    file << "ellipse_max_angle=" << config.ellipse_max_angle << std::endl;
    file << "ellipse_min_ratio=" << config.ellipse_min_ratio << std::endl;
    file << "ellipse_max_ratio=" << config.ellipse_max_ratio << std::endl;
    file << "blur_min_sigma=" << config.blur_min_sigma << std::endl;
    file << "blur_max_sigma=" << config.blur_max_sigma << std::endl;
    file << "noise_amplitude=" << config.noise_amplitude << std::endl;
    file << "margin=" << config.margin << std::endl;
    file << "seed=" << config.seed << std::endl;
    
    file.close();
}

Config loadConfig(const std::string& path) {
    Config config;
    std::ifstream file(path);
    
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << path << std::endl;
        return config;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            
            if (key == "grid_size_x") config.grid_size_x = std::stoi(value);
            else if (key == "grid_size_y") config.grid_size_y = std::stoi(value);
            else if (key == "cell_size") config.cell_size = std::stoi(value);
            else if (key == "background_brightness") config.background_brightness = std::stoi(value);
            else if (key == "ellipse_min_size") config.ellipse_min_size = std::stod(value);
            else if (key == "ellipse_max_size") config.ellipse_max_size = std::stod(value);
            else if (key == "ellipse_min_brightness") config.ellipse_min_brightness = std::stoi(value);
            else if (key == "ellipse_max_brightness") config.ellipse_max_brightness = std::stoi(value);
            else if (key == "ellipse_min_angle") config.ellipse_min_angle = std::stod(value);
            else if (key == "ellipse_max_angle") config.ellipse_max_angle = std::stod(value);
            else if (key == "ellipse_min_ratio") config.ellipse_min_ratio = std::stod(value);
            else if (key == "ellipse_max_ratio") config.ellipse_max_ratio = std::stod(value);
            else if (key == "blur_min_sigma") config.blur_min_sigma = std::stod(value);
            else if (key == "blur_max_sigma") config.blur_max_sigma = std::stod(value);
            else if (key == "noise_amplitude") config.noise_amplitude = std::stoi(value);
            else if (key == "margin") config.margin = std::stoi(value);
            else if (key == "seed") config.seed = std::stoul(value);
        }
    }
    
    return config;
}

int main(int argc, char** argv) {
    if (argc == 2) {
        Config config;
        saveConfig(argv[1], config);
        std::cout << "Default configuration saved to " << argv[1] << std::endl;
        return 0;
    } else if (argc < 3 || argc > 4) {
        std::cout << "Usage:" << std::endl;
        std::cout << "  task04-01 <config_path>   - create default config" << std::endl;
        std::cout << "  task04-01 <config_path> <image_path> <gt_path> [seed]" << std::endl;
        return -1;
    }
    
    std::string config_path = argv[1];
    std::string image_path = argv[2];
    std::string gt_path = argv[3];
    
    Config config = loadConfig(config_path);
    
    if (argc == 5) {
        config.seed = std::stoul(argv[4]);
    }
    
    std::mt19937 rng(config.seed);
    
    int width = config.grid_size_x * config.cell_size;
    int height = config.grid_size_y * config.cell_size;
    
    cv::Mat image(height, width, CV_8UC1, cv::Scalar(config.background_brightness));
    cv::Mat ground_truth(height, width, CV_8UC1, cv::Scalar(0));
    

    std::uniform_real_distribution<double> dist_size(config.ellipse_min_size, config.ellipse_max_size);
    std::uniform_int_distribution<int> dist_brightness(config.ellipse_min_brightness, config.ellipse_max_brightness);
    std::uniform_real_distribution<double> dist_angle(config.ellipse_min_angle, config.ellipse_max_angle);
    std::uniform_real_distribution<double> dist_ratio(config.ellipse_min_ratio, config.ellipse_max_ratio);
    std::uniform_real_distribution<double> dist_pos(config.margin, config.cell_size - config.margin);
    std::uniform_real_distribution<double> dist_blur(config.blur_min_sigma, config.blur_max_sigma);

    for (int y = 0; y < config.grid_size_y; y++) {
        for (int x = 0; x < config.grid_size_x; x++) {

            int cell_x = x * config.cell_size;
            int cell_y = y * config.cell_size;

            double pos_x = cell_x + dist_pos(rng);
            double pos_y = cell_y + dist_pos(rng);

            double size = dist_size(rng);
            double ratio = dist_ratio(rng);
            double angle = dist_angle(rng);
            int brightness = dist_brightness(rng);
            
            double axis1 = size;
            double axis2 = size / ratio;
            
            cv::ellipse(image, cv::Point(pos_x, pos_y), cv::Size(axis1, axis2), angle, 0, 360, cv::Scalar(brightness), -1);
            
            cv::ellipse(ground_truth, cv::Point(pos_x, pos_y), cv::Size(axis1, axis2), angle, 0, 360, cv::Scalar(255), -1);
            
            double sigma = dist_blur(rng);
            cv::GaussianBlur(image(cv::Rect(cell_x, cell_y, config.cell_size, config.cell_size)), 
                             image(cv::Rect(cell_x, cell_y, config.cell_size, config.cell_size)), 
                             cv::Size(0, 0), sigma);
        }
    }
    
    if (config.noise_amplitude > 0) {
        cv::Mat noise(height, width, CV_8SC1);
        cv::randn(noise, 0, config.noise_amplitude);
        cv::add(image, noise, image, cv::noArray(), image.type());
    }
    
    cv::imwrite(image_path, image);
    cv::imwrite(gt_path, ground_truth);
    
    std::cout << "Generated test image saved to " << image_path << std::endl;
    std::cout << "Ground truth saved to " << gt_path << std::endl;
    
    return 0;
}
