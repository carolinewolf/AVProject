#include "formkeyer.h"
#include <QDebug>
#include <vector>

using namespace cv;
using namespace std;
FormKeyer::FormKeyer()
{

}

// --- Red
// Hue
const double redHueLower1 = 0;
const double redHuerUpper1 = 20 / 2;
const double redHueLower2 = 340 / 2;
const double redHuerUpper2 = 360 / 2;
// Satturation
const double redSatLower = 100;
const double redSatUpper = 255;
// Light
const double redLigLower = 100;
const double redLigUpper = 255;


// --- Green
// Hue
const double greenHueLower = 70 / 2;
const double greenHueUpper = 120 / 2;
// Satturation
const double greenSatLower = (30 * 2.55);
const double greenSatUpper = (100 * 2.55);
// Light
const double greenLigLower = (20 * 2.55);
const double greenLigUpper = (75 * 2.55);


// --- Blue
// Hue
const double blueHueLower = 200 / 2;
const double blueHueUpper = 300 / 2;
//Satturation
const double blueSatLower = (30 * 2.55);
const double blueSatUpper = (100 * 2.55);
// Light
const double blueLigLower = (30 * 2.55);
const double blueLigUpper = (100 * 2.55);

Mat FormKeyer::process(const Mat &input){


    Mat hsvImage, redMask, greenMask, blueMask,lowerRedHueRange, upperRedHueRange;
    cvtColor(input, hsvImage, CV_BGR2HSV);


    inRange(hsvImage, Scalar(redHueLower1, 100, 100), Scalar(redHuerUpper1, 255, 255), lowerRedHueRange);
    inRange(hsvImage, Scalar(redHueLower2, 100, 100), Scalar(redHuerUpper2, 255, 255), upperRedHueRange);

    bitwise_or(lowerRedHueRange,upperRedHueRange, redMask);


    inRange(hsvImage, Scalar(greenHueLower, greenSatLower, greenLigLower), Scalar(greenHueUpper, greenSatUpper, greenLigUpper), greenMask);
    inRange(hsvImage, Scalar(blueHueLower, blueSatLower, blueLigLower), Scalar(blueHueUpper, blueSatUpper, blueLigUpper), blueMask);

    // entfernt kleine Störungen
    erode(greenMask, greenMask, Mat());
    //schließt die Objekte
    morphologyEx(greenMask,greenMask, MORPH_CLOSE, 1);

    GaussianBlur(greenMask,greenMask,Size(3,3),0);
    Mat output = detectForms(greenMask, input);

    return output;

}


Mat FormKeyer::detectForms(const Mat &mask, const Mat &input) {

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( mask, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    vector<Point> approxTriangle;
    vector<Moments> mu(contours.size());
    vector<Point2f> mc(contours.size());

    for(size_t i = 0; i < contours.size(); i++){

        approxPolyDP(contours[i], approxTriangle, arcLength(Mat(contours[i]), true)*0.05, true);
        if(approxTriangle.size() == 3){
            int center;
            drawContours(input, contours, i, Scalar(0, 255, 0), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxTriangle.begin(); vertex != approxTriangle.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
        else if(approxTriangle.size() == 4){
            drawContours(input, contours, i, Scalar(0, 0, 255), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxTriangle.begin(); vertex != approxTriangle.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
        else if(approxTriangle.size() == 5){
            drawContours(input, contours, i, Scalar(255, 0, 0), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxTriangle.begin(); vertex != approxTriangle.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
        mu[i] = moments( contours[i], false );
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        circle( input, mc[i], 4, Scalar(0,0,0), -1, 8, 0 );
    }

      return input;
}


