#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoEngine.setInputWidget(ui->inputFrame);
    videoEngine.setProcessedWidget(ui->inputFrame_2);
    videoEngine.setProcessor(&formKeyer);
    const int deviceNumber = 0;
        videoEngine.openCamera(deviceNumber + cv::CAP_DSHOW);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    videoEngine.start();
}
