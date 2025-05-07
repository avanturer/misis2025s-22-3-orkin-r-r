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
    
    Mat white_balanced;
    vector<Mat> channels;
    split(img, channels);
    
    Scalar means = mean(img);
    double avgR = means[2];
    double avgG = means[1];
    double avgB = means[0];
    double avgGray = (avgR + avgG + avgB) / 3;
    
    channels[0] = channels[0] * (avgGray / avgB);
    channels[1] = channels[1] * (avgGray / avgG);
    channels[2] = channels[2] * (avgGray / avgR);
    
    merge(channels, white_balanced);
    imwrite("output_white_balance.jpg", white_balanced);
    return 0;
}