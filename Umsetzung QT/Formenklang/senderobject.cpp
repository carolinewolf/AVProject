#include "senderobject.h"
#include <QThread>
#include "formkeyer.h"
#include <QTextStream>

using namespace std;
using namespace cv;

extern int frameHeight;
extern int frameWidth;
Point point;

SenderObject::SenderObject(QObject *parent) : QObject(parent){}

void SenderObject::DoSetup(QThread &cThread) {
    // Calculation for Framewidth and Frameheight
    frameWidthToMilli = 5000 / double(frameWidth);
    frameWidthTo10Seconds = 5 / double(frameWidth);
    frameHeightToByte = 100.0 / double(frameHeight);

    // Connect MIDI
    QStringList connections = midiOutput.connections(true);
    midiOutput.open("LoopBe Internal MIDI");

    // Connect Object to Thread
    connect(&cThread, SIGNAL(started()), this, SLOT(DoWork()));
}

void SenderObject::DoWork() {
    int countMIDI = 0;
    double lastPlayedForm = 0;
    point = Point(0, 0);

    if(forms.size() > 0) {
        // Sort Forms by x-Position
        sort(forms.begin(), forms.end(),
             [](const vector<int>& a, const vector<int>& b) {
            if(a.size() > 0 && b.size() > 0) {
                return a[1] < b[1];
            }
            return false;
        });

        // Send Forms
        for(size_t i = 0; i < forms.size(); i ++) {
            if(forms[i].size() > 0) {
                if (forms[i][0] > 0) {
                    vector<int> form = forms[i];

                    waitTime = frameWidthToMilli * double(form[1]);
                    midiX = frameWidthTo10Seconds * double(form[1]);
                    midiY =  frameHeightToByte * double(form[2]);
                    unsigned long test = (unsigned long)(waitTime - lastPlayedForm);
                    cout << "midiX: " << waitTime << " lastPlayedForm: " << lastPlayedForm << " wait: " << test << endl;
                    QThread::msleep(test);
                    lastPlayedForm = waitTime;

                    point = Point(form[1], form[2]);

                    cout << "Form: " << form[0] << ", X: " << midiX << ", Y: " << midiY << endl;
                    midiOutput.sendNoteOn(form[0], int(midiX), int(midiY));
                    countMIDI ++;
                }
            }
        }
    }
    cout << "---------------------- Gesendete MIDIs : " << countMIDI << endl << endl;
}

void SenderObject::setForms(vector<vector<int>> allForms) {
    forms = allForms;
}

void SenderObject::setImage(cv::Mat image) {
    pointedImage = image;
}
