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
    Mat gabor;
    Mat kernel = getGaborKernel(Size(21, 21), 8.0, CV_PI/4, 10.0, 0.5, 0, CV_32F);
    filter2D(img, gabor, CV_8U, kernel);
    imwrite("output_gabor.jpg", gabor);
    return 0;
}
