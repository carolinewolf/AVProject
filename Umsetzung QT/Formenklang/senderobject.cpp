#include "senderobject.h"
#include <QThread>
#include "formkeyer.h"
#include <QTextStream>

using namespace std;

extern int frameHeight;
extern int frameWidth;

SenderObject::SenderObject(QObject *parent) : QObject(parent){}

void SenderObject::DoSetup(QThread &cThread) {
    // Calculation for Framewidth and Frameheight
    frameWidthTo10Seconds = 10 / double(frameWidth);
    frameHeightToByte = 255.0 / double(frameHeight);

    // Connect MIDI
    QStringList connections = midiOutput.connections(true);
    midiOutput.open("LoopBe Internal MIDI");

    // Connect Object to Thread
    connect(&cThread, SIGNAL(started()), this, SLOT(DoWork()));
}

void SenderObject::DoWork() {
    int countMIDI = 0;
    int lastSendMIDI = 0;
    double lastPlayedForm = 0;

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
                    lastSendMIDI = forms[i][0];
                    vector<int> form = forms[i];


                    midiX = frameWidthTo10Seconds * double(form[1]);
                    midiY =  frameHeightToByte * double(form[2]);
                    cout << "Form: " << form[0] << ", X: " << midiX << ", Y: " << midiY << endl;
                    QThread::sleep(long(midiX - lastPlayedForm));
                    lastPlayedForm = midiX;

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
