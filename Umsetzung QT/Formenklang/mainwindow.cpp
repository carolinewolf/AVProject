#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QThread>
#include "senderobject.h"
#include <QTextStream>
#include <QMediaPlayer>

using namespace std;
int frameHeight;
int frameWidth;
vector<QMediaPlayer> soundPlayer(9);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Sounds Quadrate
    soundPlayer[0].setMedia(QUrl("qrc:/sounds/sounds/11.wav"));
    soundPlayer[1].setMedia(QUrl("qrc:/sounds/sounds/12.wav"));
    soundPlayer[2].setMedia(QUrl("qrc:/sounds/sounds/13.wav"));

    // Sounds Dreiecke
    soundPlayer[3].setMedia(QUrl("qrc:/sounds/sounds/21.wav"));
    soundPlayer[4].setMedia(QUrl("qrc:/sounds/sounds/22.wav"));
    soundPlayer[5].setMedia(QUrl("qrc:/sounds/sounds/23.wav"));

    // Sounds Pentagone
    soundPlayer[6].setMedia(QUrl("qrc:/sounds/sounds/31.wav"));
    soundPlayer[7].setMedia(QUrl("qrc:/sounds/sounds/32.wav"));
    soundPlayer[8].setMedia(QUrl("qrc:/sounds/sounds/33.wav"));

    // VideoEngine vorbeiten und Kamera öffnen
    videoEngine.setProcessedWidget(ui->inputFrame_2);
    videoEngine.setProcessor(&formKeyer);
    const int deviceNumber = 0;
    videoEngine.openCamera(deviceNumber + cv::CAP_DSHOW);
    videoEngine.start();

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
    QTimer *timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timerActivated()));
    timer->start(10000);
}


void MainWindow::timerActivated() {
    formKeyer.trackForms();
}

void MainWindow::on_quadrat_rot_clicked()
{
    soundPlayer[0].play();
}

void MainWindow::on_quadrat_gruen_clicked()
{
    soundPlayer[1].play();

}

void MainWindow::on_quadrat_blau_clicked()
{
    soundPlayer[2].play();
}

void MainWindow::on_dreieck_rot_clicked()
{
    soundPlayer[3].play();
}

void MainWindow::on_dreieck_gruen_clicked()
{
    soundPlayer[4].play();
}

void MainWindow::on_dreieck_blau_clicked()
{
    soundPlayer[5].play();
}

void MainWindow::on_pentagon_rot_clicked()
{
    soundPlayer[6].play();
}

void MainWindow::on_pentagon_gruen_clicked()
{
    soundPlayer[7].play();
}

void MainWindow::on_pentagon_blau_clicked()
{
    soundPlayer[8].play();
}
