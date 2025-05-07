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
    Mat sobel, prewitt, canny, laplacian;
    Sobel(img, sobel, CV_8U, 1, 0);
    
    // Аппроксимация оператора Прюитт
    Mat kernelx = (Mat_<float>(3,3) << -1,0,1,-1,0,1,-1,0,1);
    filter2D(img, prewitt, CV_8U, kernelx);
    
    Canny(img, canny, 100, 200);
    Laplacian(img, laplacian, CV_8U);
    
    imwrite("output_sobel.jpg", sobel);
    imwrite("output_prewitt.jpg", prewitt);
    imwrite("output_canny.jpg", canny);
    imwrite("output_laplacian.jpg", laplacian);
    return 0;
}
