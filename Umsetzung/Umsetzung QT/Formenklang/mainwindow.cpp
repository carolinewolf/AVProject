#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QThread>
#include "senderobject.h"
#include <QTextStream>
#include <QMediaPlayer>
#include "constants.h"

using namespace std;

vector<QMediaPlayer> soundPlayer(12);
QTimer *countDownTimer = new QTimer();
QTimer *intervalTimer = new QTimer();
bool MainWindow::isTrackingActive = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // set interval timer for tracking
    QObject::connect(intervalTimer, SIGNAL(timeout()), this, SLOT(intervalTimerActivated()));

    isCountDownActive = false;
    prepareUI();
    prepareVideoEngine();
    connectSounds();
}

void MainWindow::prepareUI() {
    ui->setupUi(this);
    ui->countDownLabel->hide();
    ui->newTrackingInLabel->hide();
}

void MainWindow::prepareVideoEngine(){
    videoEngine.setInputWidget(ui->inputImage);
    videoEngine.setProcessedWidget(ui->trackedImage);
    videoEngine.setProcessor(&formKeyer);
    const int deviceNumber = 0;
    videoEngine.openCamera(deviceNumber + cv::CAP_DSHOW);
    videoEngine.start();
}

void MainWindow::connectSounds() {
    // square
    soundPlayer[0].setMedia(QUrl("qrc:/sounds/sounds/151.wav"));
    soundPlayer[1].setMedia(QUrl("qrc:/sounds/sounds/149.wav"));
    soundPlayer[2].setMedia(QUrl("qrc:/sounds/sounds/150.wav"));
    soundPlayer[9].setMedia(QUrl("qrc:/sounds/sounds/152.wav"));

    // triangle
    soundPlayer[3].setMedia(QUrl("qrc:/sounds/sounds/157.wav"));
    soundPlayer[4].setMedia(QUrl("qrc:/sounds/sounds/155.wav"));
    soundPlayer[5].setMedia(QUrl("qrc:/sounds/sounds/156.wav"));
    soundPlayer[10].setMedia(QUrl("qrc:/sounds/sounds/158.wav"));

    // pentagone
    soundPlayer[6].setMedia(QUrl("qrc:/sounds/sounds/145.wav"));
    soundPlayer[7].setMedia(QUrl("qrc:/sounds/sounds/143.wav"));
    soundPlayer[8].setMedia(QUrl("qrc:/sounds/sounds/144.wav"));
    soundPlayer[11].setMedia(QUrl("qrc:/sounds/sounds/146.wav"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_playStopBtn_clicked()
{
    if(!MainWindow::isTrackingActive){
        MainWindow::isTrackingActive = true;
        // show label for counter
        ui->countDownLabel->show();
        ui->newTrackingInLabel->show();

        // start intervalTimer
        intervalTimer->singleShot(0, this, SLOT(intervalTimerActivated()));
        intervalTimer->start(INTERVAL_IN_MILLISECONDS);

        ui->playStopBtn->setText("Tracking stoppen");
    } else {
        intervalTimer->stop();
        formKeyer.setTrackedMatToGray();
        formKeyer.stopTracking();
        MainWindow::isTrackingActive = false;
        ui->playStopBtn->setText("Tracking starten");
        ui->countDownLabel->hide();
        ui->newTrackingInLabel->hide();
    }
}

void MainWindow::intervalTimerActivated() {
    if(isCountDownActive){
        countDownTimer->stop();
    } else {
        // Connect countDownTimer with slot-Function
        QObject::connect(countDownTimer, SIGNAL(timeout()), this, SLOT(countDown()));
        isCountDownActive = true;
    }
    counter = INTERVAL_IN_SECONDS;
    // update ui-Counter
    ui->countDownLabel->setText(QString::number(counter));
    // start timer for countdown every second
    countDownTimer->start(1000);

    // start tracking
    formKeyer.trackForms(videoEngine.getWidth(), videoEngine.getHeight());
}

void MainWindow::countDown() {
    // set counter and update ui
    counter --;
    ui->countDownLabel->setText(QString::number(counter));
}

// Slots to play sound on form
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
void MainWindow::on_quadrat_gelb_clicked()
{
    soundPlayer[9].play();
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
void MainWindow::on_dreieck_gelb_clicked()
{
    soundPlayer[10].play();
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
void MainWindow::on_pentagon_gelb_clicked()
{
    soundPlayer[11].play();
}
