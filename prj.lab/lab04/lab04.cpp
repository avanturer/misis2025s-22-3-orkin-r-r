#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    std::string image_path = DATA_DIR "/input.jpg";
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        cout << "Image not found!" << endl;
        return -1;
    }
    Mat global_thresh, otsu_thresh, adaptive_thresh;
    threshold(img, global_thresh, 127, 255, THRESH_BINARY);
    threshold(img, otsu_thresh, 0, 255, THRESH_BINARY | THRESH_OTSU);
    adaptiveThreshold(img, adaptive_thresh, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, 2);
    imwrite("output_global.jpg", global_thresh);
    imwrite("output_otsu.jpg", otsu_thresh);
    imwrite("output_adaptive.jpg", adaptive_thresh);
    return 0;
}