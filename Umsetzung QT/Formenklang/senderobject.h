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

public slots:
    void DoWork();

private:
    FormKeyer formKeyer;
    std::vector<std::vector<int>> forms;
    cv::Mat pointedImage;
    double frameWidthTo10Seconds, frameHeightToByte, frameWidthToMilli;
    double midiX, midiY, waitTime;
    drumstick::rt::MIDIOutput midiOutput;
};


#endif // SENDEROBJECT_H
