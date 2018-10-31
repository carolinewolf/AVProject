
#ifndef FORMKEYER_H
#define FORMKEYER_H

#include "videoprocessor.h"
#include <vector>

class FormKeyer : public VideoProcessor
{
public:
    FormKeyer();
    void startProcessing(const VideoFormat& format){}
    cv::Mat detectForms(const cv::Mat&mask, const cv::Mat&input);
    cv::Mat process(const cv::Mat&source);
};

#endif // COLORKEYERHSV_H
