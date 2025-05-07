#pragma once
#ifndef MISIS2025S_3_SEMCV
#define MISIS2025S_3_SEMCV

#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <filesystem>

std::string strid_from_mat(const cv::Mat& img, const int n = 4);
std::vector<std::filesystem::path> get_list_of_file_paths(const std::filesystem::path& path_lst);
cv::Mat gen_stripes(int width = 768, int height = 30, int stripe_width = 3);
cv::Mat gamma_correction(const cv::Mat& img, double gamma);

cv::Mat gen_tgtimg00(const int lev0, const int lev1, const int lev2);
cv::Mat add_noise_gau(const cv::Mat& img, const int std);
cv::Mat draw_histogram(const cv::Mat& img, const cv::Scalar& background_color = cv::Scalar(220, 220, 220));

cv::Mat autocontrast(const cv::Mat& img, const double q_black, const double q_white);
cv::Mat autocontrast_rgb(const cv::Mat& img, const double q_black, const double q_white);

#endif
