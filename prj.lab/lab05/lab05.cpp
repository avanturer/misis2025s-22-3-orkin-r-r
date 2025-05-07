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
    Mat blur_result, sobel_result, gauss_result;
    blur(img, blur_result, Size(5,5));
    GaussianBlur(img, gauss_result, Size(5,5), 1.5);
    
    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    Sobel(gray, sobel_result, CV_8U, 1, 0);
    
    imwrite("output_blur.jpg", blur_result);
    imwrite("output_sobel.jpg", sobel_result);
    imwrite("output_gauss.jpg", gauss_result);
    return 0;
}
