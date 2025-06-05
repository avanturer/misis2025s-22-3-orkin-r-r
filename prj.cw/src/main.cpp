#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "shadowledentifier.h"

using namespace cv;
using namespace std;

vector<string> getExampleImages() {
    vector<string> examples;
    
    examples.push_back("examples/shadow1.jpg");
    examples.push_back("examples/shadow2.jpg");
    examples.push_back("examples/shadow3.jpg");
    
    vector<string> valid_examples;
    for (const string& path : examples) {
        Mat test = imread(path);
        if (!test.empty()) {
            valid_examples.push_back(path);
        }
    }
    
    return valid_examples;
}

string selectImage() {
    vector<string> examples = getExampleImages();
    
    cout << "=== Shadow Segmentation Application ===" << endl;
    cout << "Choose an image to process:" << endl;
    cout << endl;
    
    if (!examples.empty()) {
        cout << "Available examples:" << endl;
        for (size_t i = 0; i < examples.size(); ++i) {
            cout << (i + 1) << ". " << examples[i] << endl;
        }
        cout << endl;
    }
    
    cout << "0. Enter custom image path" << endl;
    cout << "Enter number (1-" << examples.size() << ") or 0: ";
    
    int choice;
    cin >> choice;
    
    if (choice >= 1 && choice <= static_cast<int>(examples.size())) {
        return examples[choice - 1];
    }
    else if (choice == 0) {
        cout << "Enter image path: ";
        string custom_path;
        cin.ignore();
        getline(cin, custom_path);
        return custom_path;
    }
    else {
        cout << "Invalid choice. Try again." << endl;
        return "";
    }
}

Mat resizeForDisplay(const Mat& image, double maxWidth = 800, double maxHeight = 600) {
    if (image.empty()) return image;
    
    double scale = min(maxWidth / image.cols, maxHeight / image.rows);
    if (scale >= 1.0) return image;
    
    Mat resized;
    resize(image, resized, Size(0, 0), scale, scale);
    
    cout << "Image resized for display (scale: " << scale << ")" << endl;
    return resized;
}

Mat createColoredMask(const Mat& mask, const Mat& original) {
    Mat colored_result;
    original.copyTo(colored_result);
    
    Mat colored_mask = Mat::zeros(original.size(), CV_8UC3);
    colored_mask.setTo(Scalar(0, 0, 255), mask);
    
    addWeighted(colored_result, 0.7, colored_mask, 0.3, 0, colored_result);
    
    return colored_result;
}

int main(int argc, char** argv) {
    string image_path;
    
    if (argc == 2) {
        image_path = argv[1];
    } else {
        image_path = selectImage();
        if (image_path.empty()) {
            cout << "Exiting program." << endl;
            return 0;
        }
    }

    Mat input = imread(image_path);
    if (input.empty()) {
        cerr << "Error: Cannot load image " << image_path << endl;
        return -1;
    }

    cout << endl << "Image loaded: " << image_path << endl;
    cout << "Size: " << input.cols << "x" << input.rows << endl;

    ShadowLedentifier detector;
    Mat shadow_mask = detector.processImage(input);
    
    if (shadow_mask.empty()) {
        cerr << "Error: Failed to process image" << endl;
        return -1;
    }

    Mat colored_result = createColoredMask(shadow_mask, input);
    Mat display_original = resizeForDisplay(input);
    Mat display_mask = resizeForDisplay(shadow_mask);
    Mat display_result = resizeForDisplay(colored_result);

    cout << "Processing completed. Showing results..." << endl;
    cout << "Controls: ESC/Q - exit, S - save results" << endl;

    namedWindow("Original Image", WINDOW_AUTOSIZE);
    namedWindow("Shadow Mask", WINDOW_AUTOSIZE);
    namedWindow("Segmentation Result", WINDOW_AUTOSIZE);

    imshow("Original Image", display_original);
    imshow("Shadow Mask", display_mask);
    imshow("Segmentation Result", display_result);

    while (true) {
        int key = waitKey(30) & 0xFF;
        
        if (key == 27 || key == 'q' || key == 'Q') {
            break;
        }
        else if (key == 's' || key == 'S') {
            string base_name = image_path;
            size_t dot = base_name.find_last_of('.');
            if (dot != string::npos) {
                base_name = base_name.substr(0, dot);
            }
            
            string mask_filename = base_name + "_shadow_mask.png";
            string result_filename = base_name + "_shadow_result.png";
            
            imwrite(mask_filename, shadow_mask);
            imwrite(result_filename, colored_result);
            
            cout << "Results saved:" << endl;
            cout << "  Mask: " << mask_filename << endl;
            cout << "  Result: " << result_filename << endl;
        }
    }

    destroyAllWindows();
    return 0;
}
