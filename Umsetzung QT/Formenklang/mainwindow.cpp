#include "mainwindow.h"
#include "ui_mainwindow.h"

 int frameHeight;
 int frameWidth;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // VideoEngine vorbeiten und Kamera öffnen
    videoEngine.setProcessedWidget(ui->inputFrame_2);
    videoEngine.setProcessor(&formKeyer);
    const int deviceNumber = 1;
    videoEngine.openCamera(deviceNumber + cv::CAP_DSHOW);

    frameHeight = videoEngine.getHeight();
    frameWidth = videoEngine.getWidth();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    videoEngine.start();
}

void MainWindow::on_playStopBtn_clicked()
{

}
