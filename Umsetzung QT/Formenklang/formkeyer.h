
#ifndef FORMKEYER_H
#define FORMKEYER_H

#include "videoprocessor.h"
#include <vector>
#include "midioutput.h"
#include "form.h"

class FormKeyer : public VideoProcessor
{
public:
    FormKeyer();
    void start();
    void startProcessing(const VideoFormat& format){}
    std::vector<std::vector<int>> detectForms(const cv::Mat&mask, const int color);
    cv::Mat morphImage(const cv::Mat mask);
    cv::Mat process(const cv::Mat&source);

private:
    drumstick::rt::MIDIOutput midiOutput;
    int midichannel;
    int counter;
};

#endif // COLORKEYERHSV_H
