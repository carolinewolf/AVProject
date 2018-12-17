#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include "videoengine.h"
#include "formkeyer.h"
#include <QTime>
#include <QMediaPlayer>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void connectSounds();
    void prepareUI();
    void prepareVideoEngine();
    static bool isTrackingActive;

private slots:
    void countDown();
    void intervalTimerActivated();
    void on_playStopBtn_clicked();
    void on_quadrat_rot_clicked();
    void on_quadrat_gruen_clicked();
    void on_quadrat_blau_clicked();
    void on_quadrat_gelb_clicked();
    void on_dreieck_rot_clicked();
    void on_dreieck_gruen_clicked();
    void on_dreieck_blau_clicked();
    void on_dreieck_gelb_clicked();
    void on_pentagon_rot_clicked();
    void on_pentagon_gruen_clicked();
    void on_pentagon_blau_clicked();
    void on_pentagon_gelb_clicked();

private:
    Ui::MainWindow *ui;
    VideoEngine videoEngine;
    FormKeyer formKeyer;
    int counter;
    bool isCountDownActive;
    QThread cThread;
};

#endif
