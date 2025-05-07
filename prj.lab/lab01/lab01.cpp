#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat gammaCorrection(const Mat& img, double gamma) {
    Mat lut(1, 256, CV_8UC1);
    for (int i = 0; i < 256; i++) {
        lut.at<uchar>(i) = saturate_cast<uchar>(pow(i / 255.0, gamma) * 255.0);
    }
    Mat result;
    LUT(img, lut, result);
    return result;
}

int main() {
    std::string image_path = DATA_DIR "/input.jpg";
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        cout << "Image not found!" << endl;
        return -1;
    }
    double gamma = 2.2;
    Mat corrected = gammaCorrection(img, gamma);
    imwrite("output_gamma.jpg", corrected);
    return 0;
}
