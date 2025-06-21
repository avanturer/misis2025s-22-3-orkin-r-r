#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include "shadowledentifier.h"

using namespace cv;
using namespace std;

// Функция для пакетной обработки изображений из папки examples
int batchProcessing() {
    cout << "\n" << string(60, '=') << endl;
    cout << "    BATCH PROCESSING MODE - DEBUG OUTPUT" << endl;
    cout << string(60, '=') << endl;

    // Explicitly create debug_output folder
    std::filesystem::create_directories("debug_output");

    vector<string> image_files;
    vector<string> extensions = {".jpg", ".jpeg", ".png", ".bmp"};

    try {
        for (const auto& entry : filesystem::directory_iterator("examples")) {
            if (entry.is_regular_file()) {
                string file_ext = entry.path().extension().string();
                transform(file_ext.begin(), file_ext.end(), file_ext.begin(),
                    [](unsigned char c){ return std::tolower(c); });

                if (find(extensions.begin(), extensions.end(), file_ext) != extensions.end()) {
                    image_files.push_back(entry.path().string());
                }
            }
        }
    } catch (const exception& e) {
        cout << "Error accessing examples folder: " << e.what() << endl;
        return -1;
    }
    
    if (image_files.empty()) {
        cout << "No images found in examples/ folder!" << endl;
        return -1;
    }

    cout << "Found " << image_files.size() << " images for processing\n" << endl;

    ShadowLedentifier detector;
    int processed = 0;

    for (const string& image_path : image_files) {
        processed++;
        cout << "[" << processed << "/" << image_files.size() << "] Processing: " 
             << filesystem::path(image_path).filename().string() << endl;

        Mat input = imread(image_path);
        if (input.empty()) {
            cout << "  ✗ Failed to load image" << endl;
            continue;
        }

        string image_name = filesystem::path(image_path).stem().string();
        string debug_path = "debug_output/" + image_name;
        cout << "  [DEBUG] Output path: " << debug_path << endl;

        auto start_time = chrono::high_resolution_clock::now();
        Mat shadow_mask = detector.processImage(input, debug_path);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        // Check that at least one file is created
        bool debug_ok = false;
        try {
            debug_ok = std::filesystem::exists(debug_path + "/step5_area_filtered.jpg");
        } catch (...) {
            debug_ok = false;
        }
        if (!shadow_mask.empty() && debug_ok) {
            int total_pixels = shadow_mask.rows * shadow_mask.cols;
            int shadow_pixels = countNonZero(shadow_mask);
            double shadow_percentage = (double)shadow_pixels / total_pixels * 100;

            cout << "  ✓ Processed in " << duration.count() << " ms" << endl;
            cout << "  ✓ Shadow coverage: " << fixed << setprecision(1) << shadow_percentage << "%" << endl;
            cout << "  ✓ Debug output saved to: " << debug_path << endl;
        } else {
            cout << "  ✗ Processing failed or debug output not created!" << endl;
        }
        cout << endl;
    }

    cout << string(60, '=') << endl;
    cout << "  BATCH PROCESSING COMPLETED" << endl;
    cout << "  Processed: " << processed << "/" << image_files.size() << " images" << endl;
    cout << "  Debug output located in: debug_output/" << endl;
    cout << string(60, '=') << endl;

    // Check that debug_output folder is not empty
    bool any_debug = false;
    for (const auto& entry : std::filesystem::directory_iterator("debug_output")) {
        if (entry.is_directory()) {
            any_debug = true;
            break;
        }
    }
    if (!any_debug) {
        cout << "\n[ERROR] No debug output created! Check write permissions and processImage logic." << endl;
    }
    return 0;
}

// Остальные функции без изменений...
vector<string> getExampleImages() {
    vector<string> examples;
    examples.push_back("examples/shadow1.jpg");
    examples.push_back("examples/shadow2.jpg");
    examples.push_back("examples/shadow3.jpg");
    // Add your new images here if you want to see them in the menu
    
    vector<string> valid_examples;
    for (const string& path : examples) {
        if (!imread(path).empty()) {
            valid_examples.push_back(path);
        }
    }
    return valid_examples;
}

void printHeader() {
    cout << "\n" << string(60, '=') << endl;
    cout << "    SHADOW SEGMENTATION APPLICATION v1.0" << endl;
    cout << string(60, '=') << endl;
}

void printImageStats(const Mat& image, const string& path) {
    cout << "\n[IMAGE INFO]" << endl;
    cout << "  Path: " << path << endl;
    cout << "  Size: " << image.cols << "x" << image.rows << " pixels" << endl;
}

string selectImage() {
    vector<string> examples = getExampleImages();
    printHeader();
    cout << "\nPlease choose an image to process:\n" << endl;
    
    if (!examples.empty()) {
        cout << "Available example images:" << endl;
        for (size_t i = 0; i < examples.size(); ++i) {
            cout << "  " << (i + 1) << ". " << examples[i] << endl;
        }
        cout << endl;
    }
    
    cout << "  0. Enter custom image path" << endl;
    cout << "\nChoice (1-" << examples.size() << " or 0): ";
    
    int choice;
    cin >> choice;
    
    if (choice >= 1 && choice <= static_cast<int>(examples.size())) {
        return examples[choice - 1];
    } else if (choice == 0) {
        cout << "Enter image path: ";
        string custom_path;
        cin.ignore();
        getline(cin, custom_path);
        return custom_path;
    } else {
        cout << "Invalid choice. Please try again." << endl;
        return "";
    }
}

Mat resizeForDisplay(const Mat& image, double maxWidth = 800, double maxHeight = 600) {
    if (image.empty()) return image;
    double scale = min(maxWidth / image.cols, maxHeight / image.rows);
    if (scale >= 1.0) return image;
    Mat resized;
    resize(image, resized, Size(0, 0), scale, scale);
    return resized;
}

void printControls() {
    cout << "\n[CONTROLS]" << endl;
    cout << "  ESC / Q  - Exit application" << endl;
    cout << "  S        - Save results to current folder" << endl;
    cout << "  N        - Load next image (interactive mode)" << endl;
}

int main(int argc, char** argv) {
    if (argc == 2 && string(argv[1]) == "--batch") {
        return batchProcessing();
    }

    string image_path;
    if (argc == 2) {
        image_path = argv[1];
        cout << "[COMMAND LINE MODE]" << endl;
    } else {
        image_path = selectImage();
        if (image_path.empty()) {
            cout << "No image selected. Exiting..." << endl;
            return 0;
        }
    }

    Mat input = imread(image_path);
    if (input.empty()) {
        cerr << "ERROR: Cannot load image '" << image_path << "'" << endl;
        return -1;
    }

    printImageStats(input, image_path);

    ShadowLedentifier detector;
    
    auto start_time = chrono::high_resolution_clock::now();
    Mat shadow_mask = detector.processImage(input);
    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    
    if (shadow_mask.empty()) {
        cerr << "ERROR: Shadow segmentation failed!" << endl;
        return -1;
    }

    cout << "  Processing time: " << duration.count() << " ms" << endl;

    Mat colored_result = detector.createColoredMask(shadow_mask, input);
    Mat display_original = resizeForDisplay(input);
    Mat display_mask = resizeForDisplay(shadow_mask);
    Mat display_result = resizeForDisplay(colored_result);

    cout << "\n[SUCCESS] Processing completed!" << endl;
    printControls();

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
        } else if (key == 's' || key == 'S') {
            string base_name = image_path;
            size_t dot_pos = base_name.find_last_of('.');
            if (dot_pos != string::npos) {
                base_name = base_name.substr(0, dot_pos);
            }
            string mask_filename = base_name + "_shadow_mask.png";
            string result_filename = base_name + "_shadow_result.png";
            imwrite(mask_filename, shadow_mask);
            imwrite(result_filename, colored_result);
            cout << "\n[SAVED] Results saved successfully." << endl;
        } else if (key == 'n' || key == 'N') {
            destroyAllWindows();
            return main(1, argv);
        }
    }

    destroyAllWindows();
    cout << "Application closed." << endl;
    return 0;
}
