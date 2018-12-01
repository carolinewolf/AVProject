#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QThread>
#include "videoengine.h"
#include "formkeyer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_pushButton_clicked();
    void timerActivated();
    void on_playStopBtn_clicked();
    void on_quadrat_rot_clicked();
    void on_quadrat_gruen_clicked();
    void on_quadrat_blau_clicked();
    void on_dreieck_rot_clicked();
    void on_dreieck_gruen_clicked();
    void on_dreieck_blau_clicked();
    void on_pentagon_rot_clicked();
    void on_pentagon_gruen_clicked();
    void on_pentagon_blau_clicked();

private:
    Ui::MainWindow *ui;
    VideoEngine videoEngine;
    FormKeyer formKeyer;
QThread cThread;
};

#endif
