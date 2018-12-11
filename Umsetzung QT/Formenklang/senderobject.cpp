#include "senderobject.h"
#include <QThread>
#include "formkeyer.h"
#include "constants.h"
#include <QTextStream>

using namespace std;
using namespace cv;


Point point;

SenderObject::SenderObject(QObject *parent) : QObject(parent){}

void SenderObject::DoSetup(QThread &cThread, int frameWidth, int frameHeight) {
    // Calculation for Framewidth and Frameheight
    frameWidthToMilli = INTERVAL_IN_MILLISECONDS / double(frameWidth);
    frameWidthToSeconds = INTERVAL_IN_SECONDS / double(frameWidth);
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
    filteredForms.clear();
    if(forms.size() > 0) {
        copy_if(forms.begin(), forms.end(), back_inserter(filteredForms), [](vector<int> i){return i.size()>0;});
        // Sort Forms by x-Position
        sort(filteredForms.begin(), filteredForms.end(),
             [](const vector<int>& a, const vector<int>& b) {
            if(a.size() > 0 && b.size() > 0) {
                return a[1] < b[1];
            }
            return false;
        });

        for(size_t i = 0; i < filteredForms.size(); i ++) {
            if(filteredForms[i].size() > 0) {
                if (filteredForms[i][0] > 0) {
                    cout << filteredForms[i][1] << " " ;
                }
            }
        }

        // Send Forms
        for(size_t i = 0; i < filteredForms.size(); i ++) {
            if(filteredForms[i].size() > 0) {
                if (filteredForms[i][0] > 0) {
                    vector<int> form = filteredForms[i];

                    waitTime = frameWidthToMilli * double(form[1]);
                    midiX = frameWidthToSeconds * double(form[1]);
                    midiY =  frameHeightToByte * double(form[2]);
                    unsigned long test = (unsigned long)(waitTime - lastPlayedForm);
                    cout << "Form: " << form[0] << ", X: " << form[1] << ", MidiX: " << midiX << endl;
                    cout << "waitTime " << waitTime << " lastPlayedForm " << lastPlayedForm << " test " << test << endl;

                    // cout << "midiX: " << waitTime << " lastPlayedForm: " << lastPlayedForm << " wait: " << test << endl;
                    QThread::msleep(test);
                    lastPlayedForm = waitTime;

                    point = Point(form[1], form[2]);


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
