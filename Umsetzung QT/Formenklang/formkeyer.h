
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
    void connectMIDI();
    void startProcessing(const VideoFormat& format){}
    cv::Mat process(const cv::Mat&source);
    void createMasks();
    std::vector<std::vector<int>> detectForms(const cv::Mat&mask, const int color);
    cv::Mat morphImage(const cv::Mat mask);
    std::vector<std::vector<int>> getAllForms(cv::Mat greenMask, cv::Mat redMask, cv::Mat blueMask);
    void sendForms(std::vector<std::vector<int>> forms);


private:
    drumstick::rt::MIDIOutput midiOutput;
    int counter = 0;
    double midiX, midiY;
    bool connectedMIDI = false;
    double frameWidthToByte, frameHeightToByte;
    cv::Mat hsvImage, redMask, greenMask, blueMask,lowerRedHueMask, upperRedHueMask;

    cv::Mat output;

};

#endif // COLORKEYERHSV_H
