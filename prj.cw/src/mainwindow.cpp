#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    connectSignals();

    // Set default HSV threshold values for shadow detection
    shadowIdentifier.setHsvThresholds(0, 30, 0, 60, 0, 80);

    // Update slider values to match defaults
    hMinSlider->setValue(0);
    hMaxSlider->setValue(30);
    sMinSlider->setValue(0);
    sMaxSlider->setValue(60);
    vMinSlider->setValue(0);
    vMaxSlider->setValue(80);

    updatePreview();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUi()
{
    // Create central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create main layout
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left side - Controls
    QVBoxLayout *controlsLayout = new QVBoxLayout();

    // File operations group
    QGroupBox *fileGroup = new QGroupBox("File Operations");
    QVBoxLayout *fileLayout = new QVBoxLayout(fileGroup);

    loadButton = new QPushButton("Load Image");
    processButton = new QPushButton("Process Image");
    processButton->setEnabled(false);

    fileLayout->addWidget(loadButton);
    fileLayout->addWidget(processButton);

    // HSV Threshold controls group
    QGroupBox *hsvGroup = new QGroupBox("HSV Thresholds for Shadow Detection");
    QGridLayout *hsvLayout = new QGridLayout(hsvGroup);

    // Hue sliders
    hMinSlider = new QSlider(Qt::Horizontal);
    hMinSlider->setRange(0, 179);
    hMaxSlider = new QSlider(Qt::Horizontal);
    hMaxSlider->setRange(0, 179);

    // Saturation sliders
    sMinSlider = new QSlider(Qt::Horizontal);
    sMinSlider->setRange(0, 255);
    sMaxSlider = new QSlider(Qt::Horizontal);
    sMaxSlider->setRange(0, 255);

    // Value sliders
    vMinSlider = new QSlider(Qt::Horizontal);
    vMinSlider->setRange(0, 255);
    vMaxSlider = new QSlider(Qt::Horizontal);
    vMaxSlider->setRange(0, 255);

    // Value labels
    hMinValueLabel = new QLabel("0");
    hMaxValueLabel = new QLabel("179");
    sMinValueLabel = new QLabel("0");
    sMaxValueLabel = new QLabel("255");
    vMinValueLabel = new QLabel("0");
    vMaxValueLabel = new QLabel("255");

    // Add to layout
    hsvLayout->addWidget(new QLabel("Hue Min:"), 0, 0);
    hsvLayout->addWidget(hMinSlider, 0, 1);
    hsvLayout->addWidget(hMinValueLabel, 0, 2);

    hsvLayout->addWidget(new QLabel("Hue Max:"), 1, 0);
    hsvLayout->addWidget(hMaxSlider, 1, 1);
    hsvLayout->addWidget(hMaxValueLabel, 1, 2);

    hsvLayout->addWidget(new QLabel("Saturation Min:"), 2, 0);
    hsvLayout->addWidget(sMinSlider, 2, 1);
    hsvLayout->addWidget(sMinValueLabel, 2, 2);

    hsvLayout->addWidget(new QLabel("Saturation Max:"), 3, 0);
    hsvLayout->addWidget(sMaxSlider, 3, 1);
    hsvLayout->addWidget(sMaxValueLabel, 3, 2);

    hsvLayout->addWidget(new QLabel("Value Min:"), 4, 0);
    hsvLayout->addWidget(vMinSlider, 4, 1);
    hsvLayout->addWidget(vMinValueLabel, 4, 2);

    hsvLayout->addWidget(new QLabel("Value Max:"), 5, 0);
    hsvLayout->addWidget(vMaxSlider, 5, 1);
    hsvLayout->addWidget(vMaxValueLabel, 5, 2);

    controlsLayout->addWidget(fileGroup);
    controlsLayout->addWidget(hsvGroup);
    controlsLayout->addStretch();

    // Right side - Images
    QVBoxLayout *imagesLayout = new QVBoxLayout();

    // Input image
    QLabel *inputLabel = new QLabel("Input Image:");
    inputImageLabel = new QLabel();
    inputImageLabel->setFixedSize(400, 300);
    inputImageLabel->setStyleSheet("border: 1px solid black");
    inputImageLabel->setAlignment(Qt::AlignCenter);
    inputImageLabel->setText("No image loaded");

    // Output image
    QLabel *outputLabel = new QLabel("Shadow Segmentation Result:");
    outputImageLabel = new QLabel();
    outputImageLabel->setFixedSize(400, 300);
    outputImageLabel->setStyleSheet("border: 1px solid black");
    outputImageLabel->setAlignment(Qt::AlignCenter);
    outputImageLabel->setText("Process an image to see result");

    imagesLayout->addWidget(inputLabel);
    imagesLayout->addWidget(inputImageLabel);
    imagesLayout->addWidget(outputLabel);
    imagesLayout->addWidget(outputImageLabel);

    mainLayout->addLayout(controlsLayout);
    mainLayout->addLayout(imagesLayout);

    mainLayout->setStretchFactor(controlsLayout, 1);
    mainLayout->setStretchFactor(imagesLayout, 2);
}

void MainWindow::connectSignals()
{
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadImage);
    connect(processButton, &QPushButton::clicked, this, &MainWindow::processImage);

    // Connect sliders to update preview
    connect(hMinSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
    connect(hMaxSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
    connect(sMinSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
    connect(sMaxSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
    connect(vMinSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
    connect(vMaxSlider, &QSlider::valueChanged, this, &MainWindow::updatePreview);
}

void MainWindow::loadImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Open Image",
                                                    "",
                                                    "Image Files (*.png *.jpg *.jpeg *.bmp *.tiff)");

    if (!fileName.isEmpty()) {
        inputImage = cv::imread(fileName.toStdString());

        if (!inputImage.empty()) {
            QPixmap pixmap = matToPixmap(inputImage);
            inputImageLabel->setPixmap(pixmap.scaled(inputImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            processButton->setEnabled(true);
        } else {
            QMessageBox::warning(this, "Error", "Could not load the image.");
        }
    }
}

void MainWindow::processImage()
{
    if (inputImage.empty()) {
        QMessageBox::warning(this, "Error", "No image loaded.");
        return;
    }

    outputImage = shadowIdentifier.processImage(inputImage);

    if (!outputImage.empty()) {
        QPixmap pixmap = matToPixmap(outputImage);
        outputImageLabel->setPixmap(pixmap.scaled(outputImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void MainWindow::updatePreview()
{
    // Update labels
    hMinValueLabel->setText(QString::number(hMinSlider->value()));
    hMaxValueLabel->setText(QString::number(hMaxSlider->value()));
    sMinValueLabel->setText(QString::number(sMinSlider->value()));
    sMaxValueLabel->setText(QString::number(sMaxSlider->value()));
    vMinValueLabel->setText(QString::number(vMinSlider->value()));
    vMaxValueLabel->setText(QString::number(vMaxSlider->value()));

    // Update shadow identifier with new values
    shadowIdentifier.setHsvThresholds(hMinSlider->value(), hMaxSlider->value(),
                                      sMinSlider->value(), sMaxSlider->value(),
                                      vMinSlider->value(), vMaxSlider->value());

    // If image is loaded, process it with new parameters
    if (!inputImage.empty()) {
        processImage();
    }
}

QPixmap MainWindow::matToPixmap(const cv::Mat &mat)
{
    if (mat.channels() == 1) {
        // Grayscale image
        return QPixmap::fromImage(QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8));
    } else if (mat.channels() == 3) {
        // RGB image
        cv::Mat rgbMat;
        cv::cvtColor(mat, rgbMat, cv::COLOR_BGR2RGB);
        return QPixmap::fromImage(QImage(rgbMat.data, rgbMat.cols, rgbMat.rows, rgbMat.step, QImage::Format_RGB888));
    }
    return QPixmap();
}
