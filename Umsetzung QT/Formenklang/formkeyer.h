
#ifndef FORMKEYER_H
#define FORMKEYER_H

#include "videoprocessor.h"
#include <vector>
#include "midioutput.h"
#include <QThread>


class FormKeyer : public VideoProcessor
{
public:
    FormKeyer();
    void startProcessing(const VideoFormat& format){}
    cv::Mat process(const cv::Mat&source);
    void createMasks();
    std::vector<std::vector<int>> detectForms(const cv::Mat&mask, const int color);
    cv::Mat morphImage(const cv::Mat mask);
    std::vector<std::vector<int>> getAllForms(cv::Mat greenMask, cv::Mat redMask, cv::Mat blueMask);
    void trackForms(int frameWidth, int frameHeight);
    void stopTracking();
    void setTrackedMatToGray();
    bool hasTrackedImage = false;
    bool threadStarted = false;

private slots:

private:
    cv::Mat currentMat, trackedMat, hsvImage, redMask, greenMask, blueMask, yellowMask, lowerRedHueMask, upperRedHueMask,testImage;
    QThread sendingThread;
    std::vector<cv::Point> drawingPoints;

};

#endif // COLORKEYERHSV_H
