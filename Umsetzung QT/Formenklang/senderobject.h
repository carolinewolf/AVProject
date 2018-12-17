#ifndef SENDEROBJECT_H
#define SENDEROBJECT_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include "formkeyer.h"

class SenderObject : public QObject
{
    Q_OBJECT
public:
    explicit SenderObject(QObject *parent = nullptr);
    void DoSetup(QThread &cThread, int frameWidth, int frameHeight);
    void setForms(std::vector<std::vector<int>> forms);
    void setImage(cv::Mat image);
    std::vector<cv::Point> getPoints();

public slots:
    void DoWork();

private:
    FormKeyer formKeyer;
    std::vector<std::vector<int>> forms;
    std::vector<std::vector<int>> filteredForms;
    cv::Mat pointedImage;
    double frameWidthToSeconds, frameHeightToHundred, frameWidthToMilli;
    double midiX, midiY, totalWaitTime;
    drumstick::rt::MIDIOutput midiOutput;
    std::vector<cv::Point> points;
};


#endif // SENDEROBJECT_H
