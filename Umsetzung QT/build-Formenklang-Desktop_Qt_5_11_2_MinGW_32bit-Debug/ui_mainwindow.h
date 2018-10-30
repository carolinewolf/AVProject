/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "videowidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *playStopBtn;
    VideoWidget *inputFrame;
    VideoWidget *inputFrame_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1282, 516);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        playStopBtn = new QPushButton(centralWidget);
        playStopBtn->setObjectName(QStringLiteral("playStopBtn"));
        playStopBtn->setGeometry(QRect(340, 380, 600, 28));
        inputFrame = new VideoWidget(centralWidget);
        inputFrame->setObjectName(QStringLiteral("inputFrame"));
        inputFrame->setGeometry(QRect(0, 0, 640, 360));
        inputFrame->setFrameShape(QFrame::Box);
        inputFrame->setScaledContents(false);
        inputFrame->setAlignment(Qt::AlignCenter);
        inputFrame_2 = new VideoWidget(centralWidget);
        inputFrame_2->setObjectName(QStringLiteral("inputFrame_2"));
        inputFrame_2->setGeometry(QRect(642, 0, 640, 360));
        inputFrame_2->setFrameShape(QFrame::Box);
        inputFrame_2->setScaledContents(false);
        inputFrame_2->setAlignment(Qt::AlignCenter);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1282, 26));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        playStopBtn->setText(QApplication::translate("MainWindow", "Tracking starten", nullptr));
        inputFrame->setText(QString());
        inputFrame_2->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
