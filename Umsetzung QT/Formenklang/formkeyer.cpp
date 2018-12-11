#include "formkeyer.h"
#include <QDebug>
#include <vector>
#include "form.h"
#include <QTextStream>
#include <QTime>
#include <QFuture>
#include <QTimer>
#include <QtConcurrent/QtConcurrent>
#include <windows.h> // for Sleep
#include <QObject>
#include <senderobject.h>
#include <QCoreApplication>
#include <constants.h>

using namespace cv;
using namespace std;

SenderObject cObject;
extern int isTracking;
bool threadStarted = false;
bool hasTrackedImage = false;
extern Point point;

FormKeyer::FormKeyer(){}


Mat FormKeyer::process(const Mat &input){
    Mat image(input.rows, input.cols, input.type(), Scalar(100,100,100));
    if(!hasTrackedImage) {
        image.copyTo(trackedMat);
        hasTrackedImage = true;
    }
    input.copyTo(actualMat);


    if (point.x > 0) {
        circle(trackedMat, point, 5, cv::Scalar(255,255,255), CV_FILLED);
    }
    return trackedMat;
}

void FormKeyer::trackForms() {
    if (!threadStarted) {
        cObject.DoSetup(cThread);
        cObject.moveToThread(&cThread);
        threadStarted = true;
    } else {
        cThread.quit();
    }
    cvtColor(actualMat, hsvImage, CV_BGR2HSV);
    createMasks();
    vector<vector<int>> allForms = getAllForms(greenMask,redMask,blueMask);

    actualMat.copyTo(trackedMat);
    cObject.setForms(allForms);
    cThread.start();
}

void FormKeyer::createMasks() {

    // Red Mask
    inRange(hsvImage, Scalar(redHueLower1, redSatLower, redLigLower), Scalar(redHuerUpper1, redSatUpper, redLigUpper), lowerRedHueMask);
    inRange(hsvImage, Scalar(redHueLower2, redSatLower, redLigLower), Scalar(redHuerUpper2, redSatUpper, redLigUpper), upperRedHueMask);
    bitwise_or(lowerRedHueMask,upperRedHueMask, redMask);

    // Green Mask
    inRange(hsvImage, Scalar(greenHueLower, greenSatLower, greenLigLower), Scalar(greenHueUpper, greenSatUpper, greenLigUpper), greenMask);

    // Blue Mask
    inRange(hsvImage, Scalar(blueHueLower, blueSatLower, blueLigLower), Scalar(blueHueUpper, blueSatUpper, blueLigUpper), blueMask);

    // Black Mask
    inRange(hsvImage, Scalar(blackHueLower, blackSatLower, blackLigLower), Scalar(blackHueUpper, blackSatUpper, blackLigUpper), blackMask);


    // Morph Masks
    blueMask = morphImage(blueMask);
    greenMask = morphImage(greenMask);
    redMask = morphImage(redMask);
    blackMask = morphImage(blackMask);

}

Mat FormKeyer::morphImage(Mat mask) {
    int morph_size = 2;
    Mat element = getStructuringElement( MORPH_ELLIPSE, Size( 4*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(mask, mask, MORPH_OPEN,element);
    return mask;
}

vector<vector<int>> FormKeyer::getAllForms(Mat greenMask, Mat redMask, Mat blueMask) {
    vector<vector<int>> greenForms = detectForms(greenMask, GREEN_FORM);
    vector<vector<int>> redForms = detectForms(redMask, RED_FORM);
    vector<vector<int>> blueForms = detectForms(blueMask, BLUE_FORM);
    vector<vector<int>> blackForms = detectForms(blackMask, BLACK_FORM);
    vector<vector<int>> allForms;

    // Combine Forms
    allForms.reserve( greenForms.size() + redForms.size() + blueForms.size() + blackForms.size() );
    allForms.insert( allForms.end(), greenForms.begin(), greenForms.end() );
    allForms.insert( allForms.end(), redForms.begin(), redForms.end() );
    allForms.insert( allForms.end(), blueForms.begin(), blueForms.end() );
    allForms.insert( allForms.end(), blackForms.begin(), blackForms.end() );
    return allForms;
}

vector<vector<int>> FormKeyer::detectForms(const Mat &mask, const int color) {
    int formType, x, y;
    vector<vector<Point>> contours;
    vector<Point> approxPoly;

    findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<int>> forms (contours.size());

    String colorText;
    if (color == 1) {colorText = "Gruen";}
    else if (color == 2) {colorText = "Blau";}
    else if (color == 3) {colorText = "Rot";}
    else if (color == 4) {colorText = "Schwarz";}

    for(size_t i = 0; i < contours.size(); i++){
        if(contours[i].size() > 20){

            Rect bound = boundingRect(Mat(contours[i]));
            x = (bound.x + (bound.width/2));
            y = (bound.y + (bound.height/2));

            if (bound.width > 20 && bound.height > 20){
                approxPolyDP(contours[i], approxPoly, arcLength(Mat(contours[i]), true)*0.05, true);
                double area = contourArea(approxPoly);
                if (area > 500) {
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
                    if(formType != 0) {
                        vector<int> form = {formType, x, y};
                        forms[i] = form;
                    }

                }
            }
        }
    }
    return forms;
}
