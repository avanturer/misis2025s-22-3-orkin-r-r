#include <opencv2/opencv.hpp>
#include <iostream>
#include <random>

using namespace cv;
using namespace std;

void addGaussianNoise(Mat& img, double mean, double stddev) {
    Mat noise(img.size(), img.type());
    randn(noise, mean, stddev);
    img += noise;
}

void addSaltPepperNoise(Mat& img, double amount) {
    int num_salt = static_cast<int>(amount * img.total());
    for (int i = 0; i < num_salt; i++) {
        int r = rand() % img.rows;
        int c = rand() % img.cols;
        if (img.channels() == 1) {
            img.at<uchar>(r, c) = 255;
        } else {
            for (int ch = 0; ch < img.channels(); ch++) {
                img.at<Vec3b>(r, c)[ch] = 255;
            }
        }
    }
}

int main() {
    std::string image_path = DATA_DIR "/input.jpg";
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        cout << "Image not found!" << endl;
        return -1;
    }
    addGaussianNoise(img, 0, 30);
    addSaltPepperNoise(img, 0.01);
    imwrite("output_noise.jpg", img);
    return 0;
}
