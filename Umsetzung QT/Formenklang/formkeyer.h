
#ifndef FORMKEYER_H
#define FORMKEYER_H

#include "videoprocessor.h"

class FormKeyer : public VideoProcessor
{
public:
    FormKeyer();
    void startProcessing(const VideoFormat& format){}
    cv::Mat process(const cv::Mat&source);
};

#endif // COLORKEYERHSV_H
