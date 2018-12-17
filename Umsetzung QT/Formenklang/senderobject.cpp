#include "senderobject.h"
#include <QThread>
#include "formkeyer.h"
#include "constants.h"
#include "mainwindow.h"
#include <QTextStream>

using namespace std;
using namespace cv;


SenderObject::SenderObject(QObject *parent) : QObject(parent){}

void SenderObject::DoSetup(QThread &cThread, int frameWidth, int frameHeight) {
    // Calculation for Framewidth and Frameheight
    frameWidthToMilli = INTERVAL_IN_MILLISECONDS / double(frameWidth);
    frameWidthToSeconds = INTERVAL_IN_SECONDS / double(frameWidth);
    frameHeightToHundred = 100.0 / double(frameHeight);

    // Connect MIDI
    QStringList connections = midiOutput.connections(true);
    midiOutput.open("LoopBe Internal MIDI");

    // Connect Object to Thread
    connect(&cThread, SIGNAL(started()), this, SLOT(DoWork()));
}

void SenderObject::DoWork() {
    // prepare and reset for work
    double lastPlayedFormTime = 0;
    points.clear();
    filteredForms.clear();

    if(forms.size() > 0) {
        // filter forms where i.size > 0
        copy_if(forms.begin(), forms.end(), back_inserter(filteredForms), [](vector<int> i){return i.size()>0;});

        // Sort Forms by x-Position
        sort(filteredForms.begin(), filteredForms.end(),
             [](const vector<int>& a, const vector<int>& b) {
            if(a.size() > 0 && b.size() > 0) {
                return a[1] < b[1];
            }
            return false;
        });

        // Send Forms
        for(size_t i = 0; i < filteredForms.size(); i ++) {
            if(filteredForms[i].size() > 0) {
                if (filteredForms[i][0] > 0) {
                    vector<int> form = filteredForms[i];

                    // calculate total wait time and calculated wait time
                    totalWaitTime = frameWidthToMilli * double(form[1]);
                    unsigned long calculatedWaitTime = (unsigned long)(totalWaitTime - lastPlayedFormTime);

                    // calculate midiX and midiY
                    midiX = frameWidthToSeconds * double(form[1]);
                    midiY =  frameHeightToHundred * double(form[2]);

                    // wait calculated wait time and set it to last time
                    QThread::msleep(calculatedWaitTime);
                    lastPlayedFormTime = totalWaitTime;

                    if (MainWindow::isTrackingActive) {
                        // set point for drawing in formkeyer
                        points.push_back(Point(form[1], form[2]));

                        // send midi to LoopBe
                        midiOutput.sendNoteOn(form[0], int(midiX), int(midiY));
                    }
                }
            }
        }
        cout << "--------------------------------- all sended" << endl;
    }
}

void SenderObject::setForms(vector<vector<int>> allForms) {
    forms = allForms;
}

void SenderObject::setImage(cv::Mat image) {
    pointedImage = image;
}

vector<Point> SenderObject::getPoints() {
    return points;
}
