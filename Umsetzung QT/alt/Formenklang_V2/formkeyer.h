
#ifndef FORMKEYER_H
#define FORMKEYER_H

#include "videoprocessor.h"
#include <vector>
#include "midioutput.h"

class FormKeyer : public VideoProcessor
{
public:
    FormKeyer();
    void start();
    void startProcessing(const VideoFormat& format){}
    std::vector<std::vector<cv::Point>> detectForms(const cv::Mat&mask, const cv::Mat&input);
    cv::Mat process(const cv::Mat&source);

private:
    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;
};

#endif // COLORKEYERHSV_H
