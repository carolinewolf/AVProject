#include "formkeyer.h"
#include <QDebug>
#include <vector>
#include "form.h"
#include <QTextStream>
#include <constants.h>

extern int frameHeight;
extern int frameWidth;

using namespace cv;
using namespace std;

FormKeyer::FormKeyer(){}

int counter = 0;


double xA, yA;

bool connectedMIDI = false;
void FormKeyer::start(){
    QStringList connections = midiOutput.connections(true);
    midiOutput.open("LoopBe Internal MIDI");

}
Mat output;

Mat FormKeyer::process(const Mat &input){

    if(!connectedMIDI) {
        start();
        connectedMIDI = true;
        input.copyTo(output);
    }


    if (counter > 100) {
        cout << "-------------------------------------------" << endl;

        Mat hsvImage, redMask, greenMask, blueMask,lowerRedHueRange, upperRedHueRange;

        cvtColor(input, hsvImage, CV_BGR2HSV);

        // Nach Rot suchen - oberer und unterer Bereich der 360 Grad
        inRange(hsvImage, Scalar(redHueLower1, redSatLower, redLigLower), Scalar(redHuerUpper1, redSatUpper, redLigUpper), lowerRedHueRange);
        inRange(hsvImage, Scalar(redHueLower2, redSatLower, redLigLower), Scalar(redHuerUpper2, redSatUpper, redLigUpper), upperRedHueRange);
        // Verunden der beiden Rotbereiche
        bitwise_or(lowerRedHueRange,upperRedHueRange, redMask);

        // Nach Grün suchen
        inRange(hsvImage, Scalar(greenHueLower, greenSatLower, greenLigLower), Scalar(greenHueUpper, greenSatUpper, greenLigUpper), greenMask);
        // Nach Blau suchen
        inRange(hsvImage, Scalar(blueHueLower, blueSatLower, blueLigLower), Scalar(blueHueUpper, blueSatUpper, blueLigUpper), blueMask);


        blueMask = morphImage(blueMask);
        greenMask = morphImage(greenMask);
        redMask = morphImage(redMask);

        vector<vector<int>> greenForms = detectForms(greenMask, GREEN_FORM);
        vector<vector<int>> redForms = detectForms(redMask, RED_FORM);
        vector<vector<int>> blueForms = detectForms(blueMask, BLUE_FORM);

        vector<vector<int>> allForms;

        allForms.reserve( greenForms.size() + redForms.size() + blueForms.size() );
        allForms.insert( allForms.end(), greenForms.begin(), greenForms.end() );
        allForms.insert( allForms.end(), redForms.begin(), redForms.end() );
        allForms.insert( allForms.end(), blueForms.begin(), blueForms.end() );

        double berechnungW = 255.0 / double(frameWidth);
        double berechnungH = 255.0 / double(frameHeight);

        if(allForms.size() > 0) {
            for(size_t i = 0; i < allForms.size(); i ++) {
                if(allForms[i].size() > 0) {
                    if (allForms[i][0] > 0) {
                        vector<int> form = allForms[i];
                        // Umrechnung auf max 255
                        xA = berechnungW * double(form[1]);
                        yA =  berechnungH * double(form[2]);
                        cout << "Form: " << form[0] << ", X: " << xA << ", Y: " << yA << endl;
                        midiOutput.sendNoteOn(form[0], int(xA), int(yA));
                    }
                }
            }
        }
        counter = 0;
        redMask.copyTo(output);
        return output;
    } else {
        counter += 1;
        return output;
    }

}


Mat FormKeyer::morphImage(Mat mask) {
    int morph_size = 2;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 4*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(mask, mask, MORPH_OPEN,element);
    return mask;
}

vector<vector<int>> FormKeyer::detectForms(const Mat &mask, const int color) {

    int formType, x, y;
    vector<vector<Point> > contours;
    vector<Point> approxPoly;
    findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<vector<int>> forms (contours.size());
    String colorText;
    if (color == 1) {
        colorText = "Gruen";
    } else if (color == 2) {
        colorText = "Blau";
    } else if (color == 3) {
        colorText = "Rot";
    }
    for(size_t i = 0; i < contours.size(); i++){

        if(contours[i].size() > 20){
            Rect bound = boundingRect(Mat(contours[i]));
            x = (bound.x + (bound.width/2));
            y = (bound.y + (bound.height/2));
            if (bound.width > 20 && bound.height > 20){
                approxPolyDP(contours[i], approxPoly, arcLength(Mat(contours[i]), true)*0.05, true);

                switch(approxPoly.size()) {
                case 3 : formType = TRIANGLE_NUMBER + color;
                    cout << "----------" << endl;
                    cout << "DREIECK " << colorText << endl;
                    cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;


                    break;
                case 4 : formType = QUADER_NUMBER + color;
                    cout << "----------" << endl;
                    cout << "QUADRAT " << colorText << endl;
                    cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;


                    break;
                case 5 : formType = PENTAGON_NUMBER + color;
                    cout << "----------" << endl;
                    cout << "PENTAGON " << colorText << endl;
                    cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;


                    break;
                default:
                    formType = 0;
                    x = 0;
                    y = 0;
                    break;
                }

                vector<int> form = {formType, x, y};
                forms[i] = form;
            }
        }
    }
    return forms;
}


