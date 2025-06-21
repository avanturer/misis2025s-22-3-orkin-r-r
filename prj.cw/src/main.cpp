#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <iomanip>
#include "shadowledentifier.h"
#include <chrono>

using namespace cv;
using namespace std;

int batchProcessing() {
    cout << "\n" << string(60, '=') << endl;
    cout << "    BATCH PROCESSING MODE - DEBUG OUTPUT" << endl;
    cout << string(60, '=') << endl;

    vector<string> image_files;
    
    // Найти все изображения в папке examples
    vector<string> extensions = {".jpg", ".jpeg", ".png", ".bmp"};
    for (const auto& ext : extensions) {
        string pattern = "examples/*" + ext;
        // Простой поиск файлов
        for (const auto& entry : filesystem::directory_iterator("examples")) {
            if (entry.is_regular_file()) {
                string file_ext = entry.path().extension().string();
                transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::tolower);
                if (find(extensions.begin(), extensions.end(), file_ext) != extensions.end()) {
                    image_files.push_back(entry.path().string());
                }
            }
        }
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

        // Генерация пути для debug вывода
        string image_name = filesystem::path(image_path).stem().string();
        string debug_path = "debug_output/" + image_name;

        auto start_time = chrono::high_resolution_clock::now();
        Mat shadow_mask = detector.processImage(input, debug_path);
        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        if (!shadow_mask.empty()) {
            int total_pixels = shadow_mask.rows * shadow_mask.cols;
            int shadow_pixels = countNonZero(shadow_mask);
            double shadow_percentage = (double)shadow_pixels / total_pixels * 100;

            cout << "  ✓ Processed in " << duration.count() << " ms" << endl;
            cout << "  ✓ Shadow coverage: " << fixed << setprecision(1) 
                 << shadow_percentage << "%" << endl;
            cout << "  ✓ Debug files saved to: " << debug_path << "/" << endl;
        } else {
            cout << "  ✗ Processing failed" << endl;
        }
        cout << endl;
    }

    cout << string(60, '=') << endl;
    cout << "  BATCH PROCESSING COMPLETED" << endl;
    cout << "  Processed: " << processed << "/" << image_files.size() << " images" << endl;
    cout << "  Debug output: debug_output/" << endl;
    cout << string(60, '=') << endl;

    return 0;
}


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

void printHeader() {
    cout << "\n" << string(60, '=') << endl;
    cout << "    SHADOW SEGMENTATION APPLICATION v1.0" << endl;
    cout << "         Computer Vision Course Project" << endl;
    cout << string(60, '=') << endl;
}

void printImageStats(const Mat& image, const string& path) {
    cout << "\n[IMAGE INFO]" << endl;
    cout << "  Path: " << path << endl;
    cout << "  Size: " << image.cols << "x" << image.rows << " pixels" << endl;
    cout << "  Channels: " << image.channels() << endl;
    double size_mb = (image.cols * image.rows * image.channels()) / (1024.0 * 1024.0);
    cout << "  Memory: " << fixed << setprecision(2) << size_mb << " MB" << endl;
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
    }
    else if (choice == 0) {
        cout << "Enter image path: ";
        string custom_path;
        cin.ignore();
        getline(cin, custom_path);
        return custom_path;
    }
    else {
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
    
    cout << "  Display scale: " << fixed << setprecision(3) << scale << endl;
    return resized;
}

void printProcessingInfo() {
    cout << "\n[PROCESSING]" << endl;
    cout << "  Algorithm: HSV + LAB color space analysis" << endl;
    cout << "  HSV V-threshold: ≤ 120" << endl;
    cout << "  LAB L-threshold: ≤ 140" << endl;
    cout << "  Morphology kernel: 5x5 ellipse" << endl;
    cout << "  Min contour area: 100 pixels" << endl;
    cout << "  Processing..." << endl;
}

void printControls() {
    cout << "\n[CONTROLS]" << endl;
    cout << "  ESC / Q  - Exit application" << endl;
    cout << "  S        - Save results to current folder" << endl;
    cout << "  N        - Load next image (interactive mode)" << endl;
    cout << "  Any key  - Continue viewing" << endl;
    cout << "\nPress any key in the image windows to control..." << endl;
}

int main(int argc, char** argv) {
    // Проверка на режим пакетной обработки
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
        cerr << "Please check if the file exists and is a valid image format." << endl;
        return -1;
    }

    printImageStats(input, image_path);
    printProcessingInfo();

    ShadowLedentifier detector;

    // Generate debug path
    std::string image_name = image_path;
    size_t last_slash = image_name.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        image_name = image_name.substr(last_slash + 1);
    }
    size_t dot = image_name.find_last_of('.');
    if (dot != std::string::npos) {
        image_name = image_name.substr(0, dot);
    }

    std::string debug_path = "debug_output/" + image_name;
    auto start_time = chrono::high_resolution_clock::now();
    
    Mat shadow_mask = detector.processImage(input, debug_path);
    
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
            cout << "\nExiting application..." << endl;
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
            
            cout << "\n[SAVED] Results saved successfully:" << endl;
            cout << "  Mask: " << mask_filename << endl;
            cout << "  Result: " << result_filename << endl;
        }
        else if (key == 'n' || key == 'N') {  // ДОБАВИТЬ переключение к следующему изображению
            cout << "\nLoading next image..." << endl;
            destroyAllWindows();
            
            // Простой перезапуск с интерактивным выбором
            return main(1, argv);
        }
    }

    destroyAllWindows();
    cout << "Application closed successfully." << endl;
    return 0;
}