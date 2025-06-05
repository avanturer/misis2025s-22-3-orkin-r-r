#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <opencv2/opencv.hpp>
#include "shadowledentifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void loadImage();
    void processImage();
    void updatePreview();

private:
    // UI elements
    QLabel *inputImageLabel;
    QLabel *outputImageLabel;
    QPushButton *loadButton;
    QPushButton *processButton;

    // Sliders for HSV thresholding
    QSlider *hMinSlider;
    QSlider *hMaxSlider;
    QSlider *sMinSlider;
    QSlider *sMaxSlider;
    QSlider *vMinSlider;
    QSlider *vMaxSlider;

    // Labels for slider values
    QLabel *hMinValueLabel;
    QLabel *hMaxValueLabel;
    QLabel *sMinValueLabel;
    QLabel *sMaxValueLabel;
    QLabel *vMinValueLabel;
    QLabel *vMaxValueLabel;

    // Image data
    cv::Mat inputImage;
    cv::Mat outputImage;

    // Shadow identifier
    ShadowIdentifier shadowIdentifier;

    // Helper functions
    void setupUi();
    void connectSignals();
    QPixmap matToPixmap(const cv::Mat &mat);
};

#endif // MAINWINDOW_H
