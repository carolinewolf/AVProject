#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <vector>
#include "senderobject.h"
#include "formkeyer.h"
#include "constants.h"
#include "mainwindow.h"

using namespace cv;
using namespace std;

SenderObject senderObject;
extern int isTracking;

FormKeyer::FormKeyer(){}

Mat FormKeyer::process(const Mat &input){
    // prepare tracked image as gray image
    if(!hasTrackedImage) {
        Mat grayImage(input.rows, input.cols, input.type(), Scalar(100,100,100));
        grayImage.copyTo(trackedMat);
        hasTrackedImage = true;
    }

    // copy input to actualMat
    input.copyTo(currentMat);

    if(MainWindow::isTrackingActive){
        // get and draw point for played form
        drawingPoints = senderObject.getPoints();
        for(size_t i = 0; i < drawingPoints.size(); i++){
            circle(trackedMat, drawingPoints[i], 2, cv::Scalar(255,255,255), CV_FILLED);
        }
    }

    // return output Mat
    return trackedMat;
}

void FormKeyer::trackForms(int frameWidth, int frameHeight) {

    // do setup for thread, else quit thread
    if (!threadStarted) {
        senderObject.DoSetup(sendingThread, frameWidth, frameHeight);
        senderObject.moveToThread(&sendingThread);
        threadStarted = true;
    } else {
        sendingThread.quit();
        drawingPoints.clear();
    }

    // convert current Mat from frame, create masks for each color and collect all forms
    cvtColor(currentMat, hsvImage, CV_BGR2HSV);
    createMasks();
    vector<vector<int>> allForms = getAllForms(greenMask,redMask,blueMask, yellowMask);

    currentMat.copyTo(trackedMat);

    // set forms for sending thread and start its
    senderObject.setForms(allForms);
    sendingThread.start();
}

void FormKeyer::setTrackedMatToGray() {
    Mat image(trackedMat.rows, trackedMat.cols, trackedMat.type(), Scalar(100,100,100));
    image.copyTo(trackedMat);
}

void FormKeyer::stopTracking(){
    sendingThread.terminate();
}

void FormKeyer::createMasks() {
    // Red Mask
    inRange(hsvImage, Scalar(RED_HUE_LOWER_1, RED_SAT_LOWER, RED_LIGHT_LOWER), Scalar(RED_HUE_UPPER_1, RED_SAT_UPPER, RED_LIGHT_UPPER), lowerRedHueMask);
    inRange(hsvImage, Scalar(RED_HUE_LOWER_2, RED_SAT_LOWER, RED_LIGHT_LOWER), Scalar(RED_HUE_UPPER_2, RED_SAT_UPPER, RED_LIGHT_UPPER), upperRedHueMask);
    bitwise_or(lowerRedHueMask,upperRedHueMask, redMask);

    // Green Mask
    inRange(hsvImage, Scalar(GREEN_HUE_LOWER, GREEN_SAT_LOWER, GREEN_LIGHT_LOWER), Scalar(GREEN_HUE_UPPER, GREEN_SAT_UPPER, GREEN_LIGHT_UPPER), greenMask);

    // Blue Mask
    inRange(hsvImage, Scalar(BLUE_HUE_LOWER, BLUE_SAT_LOWER, BLUE_LIGHT_LOWER), Scalar(BLUE_HUE_UPPER, BLUE_SAT_UPPER, BLUE_LIGHT_UPPER), blueMask);

    // Yellow Mask
    inRange(hsvImage, Scalar(YELLOW_HUE_LOWER, YELLOW_SAT_LOWER, YELLOW_LIGHT_LOWER), Scalar(YELLOW_HUE_UPPER, YELLOW_SAT_UPPER, YELLOW_LIGHT_UPPER), yellowMask);

    // Morph Masks
    blueMask = morphImage(blueMask);
    greenMask = morphImage(greenMask);
    redMask = morphImage(redMask);
    yellowMask = morphImage(yellowMask);

}

Mat FormKeyer::morphImage(Mat mask) {
    int morph_size = 2;
    Mat structuringElement = getStructuringElement( MORPH_ELLIPSE, Size( 4*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    morphologyEx(mask, mask, MORPH_OPEN, structuringElement);
    return mask;
}

vector<vector<int>> FormKeyer::getAllForms(Mat greenMask, Mat redMask, Mat blueMask, Mat yellowMask) {
    vector<vector<int>> greenForms = detectForms(greenMask, GREEN_FORM);
    vector<vector<int>> redForms = detectForms(redMask, RED_FORM);
    vector<vector<int>> blueForms = detectForms(blueMask, BLUE_FORM);
    vector<vector<int>> yellowForms = detectForms(yellowMask, YELLOW_FORM);
    vector<vector<int>> allForms;

    // Combine color forms into one array
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
    vector<vector<Point>> filteredContours;
    vector<Point> approxPoly;

    // find contours from mask
    findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    copy_if(contours.begin(), contours.end(), back_inserter(filteredContours), [](vector<Point> i){return i.size()>20;});

    vector<vector<int>> forms(filteredContours.size());

    // only for console log necessary
    String colorText;
    if (color == 1) {colorText = "Gruen";}
    else if (color == 2) {colorText = "Blau";}
    else if (color == 3) {colorText = "Rot";}
    else if (color == 4) {colorText = "Gelb";}

    // loop through all contours
    for(size_t i = 0; i < filteredContours.size(); i++){
        // find bounding rect from contour and calculate x and y position
        Rect bound = boundingRect(Mat(filteredContours[i]));
        x = (bound.x + (bound.width/2));
        y = (bound.y + (bound.height/2));

        if (bound.width > 30 && bound.height > 30){
            // approx poly from contours and contour the poly to get vertices
            approxPolyDP(filteredContours[i], approxPoly, arcLength(Mat(filteredContours[i]), true)*0.05, true);
            double area = contourArea(approxPoly);
            if (area > 400) {
                // assign form and color
                switch(approxPoly.size()) {
                case 3 : formType = TRIANGLE_NUMBER + color;
                    // only for console log necessary
                    cout << "----------" << endl;
                    cout << "DREIECK " << colorText << endl;
                    cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;
                    break;
                case 4 : formType = QUADER_NUMBER + color;
                    // only for console log necessary
                    cout << "----------" << endl;
                    cout << "QUADRAT " << colorText << endl;
                    cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;
                    break;
                case 5 : formType = PENTAGON_NUMBER + color;
                    // only for console log necessary
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
    return forms;
}
