#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <vector>
#include "senderobject.h"
#include "formkeyer.h"
#include "constants.h"

using namespace cv;
using namespace std;

SenderObject senderObject;
extern int isTracking;
extern Point point;

FormKeyer::FormKeyer(){}
/*
void FormKeyer::startProcessing(const VideoFormat &format) {

}
*/

Mat FormKeyer::process(const Mat &input){
    if(!hasTrackedImage) {
        Mat image(input.rows, input.cols, input.type(), Scalar(100,100,100));
        image.copyTo(trackedMat);
        hasTrackedImage = true;
    }
    input.copyTo(actualMat);

    // draw point for played form
    if (point.x > 0) {
        circle(trackedMat, point, 5, cv::Scalar(255,255,255), CV_FILLED);
    }
    return trackedMat;
}

void FormKeyer::setTrackedMat() {
    Mat image(trackedMat.rows, trackedMat.cols, trackedMat.type(), Scalar(100,100,100));
    image.copyTo(trackedMat);
}

void FormKeyer::trackForms(int frameWidth, int frameHeight) {
    if (!threadStarted) {
        senderObject.DoSetup(cThread, frameWidth, frameHeight);
        senderObject.moveToThread(&cThread);
        threadStarted = true;
    } else {
        cThread.quit();
    }
    cvtColor(actualMat, hsvImage, CV_BGR2HSV);
    createMasks();
    vector<vector<int>> allForms = getAllForms(greenMask,redMask,blueMask);

    actualMat.copyTo(trackedMat);
    senderObject.setForms(allForms);
    cThread.start();
}

void FormKeyer::stopTracking(){
    cThread.terminate();
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

    // yellow Mask
    inRange(hsvImage, Scalar(yellowHueLower, yellowSatLower, yellowLigLower), Scalar(yellowHueUpper, yellowSatUpper, yellowLigUpper), yellowMask);

    // Morph Masks
    blueMask = morphImage(blueMask);
    greenMask = morphImage(greenMask);
    redMask = morphImage(redMask);
    yellowMask = morphImage(yellowMask);

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
    vector<vector<int>> yellowForms = detectForms(yellowMask, YELLOW_FORM);
    vector<vector<int>> allForms;

    // Combine Forms
    allForms.reserve( greenForms.size() + redForms.size() + blueForms.size() + yellowForms.size() );
    allForms.insert( allForms.end(), greenForms.begin(), greenForms.end() );
    allForms.insert( allForms.end(), redForms.begin(), redForms.end() );
    allForms.insert( allForms.end(), blueForms.begin(), blueForms.end() );
    allForms.insert( allForms.end(), yellowForms.begin(), yellowForms.end() );
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
