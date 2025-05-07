#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat linearStretch(const Mat& img) {
    double minVal, maxVal;
    minMaxLoc(img, &minVal, &maxVal);
    Mat result;
    img.convertTo(result, CV_8U, 255.0 / (maxVal - minVal), -minVal * 255.0 / (maxVal - minVal));
    return result;
}

Mat histogramEqualization(const Mat& img) {
    Mat result;
    if (img.channels() == 1) {
        equalizeHist(img, result);
    } else {
        Mat ycrcb;
        cvtColor(img, ycrcb, COLOR_BGR2YCrCb);
        vector<Mat> channels;
        split(ycrcb, channels);
        equalizeHist(channels[0], channels[0]);
        merge(channels, ycrcb);
        cvtColor(ycrcb, result, COLOR_YCrCb2BGR);
    }
    return result;
}

int main() {
    std::string image_path = DATA_DIR "/input.jpg";
    cv::Mat img = cv::imread(image_path);
    if (img.empty()) {
        cout << "Image not found!" << endl;
        return -1;
    }
    Mat stretched = linearStretch(img);
    Mat equalized = histogramEqualization(img);
    imwrite("output_stretched.jpg", stretched);
    imwrite("output_equalized.jpg", equalized);
    return 0;
}
