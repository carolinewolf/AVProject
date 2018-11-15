#include "formkeyer.h"
#include <QDebug>
#include <vector>
#include "form.h"


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

int counter = 0;

const int TRIANGLE_NUMBER = 10;
const int QUADER_NUMBER = 20;
const int PENTAGON_NUMBER = 30;

bool connectedMIDI = false;
void FormKeyer::start(){
    QStringList connections = midiOutput.connections(true);
    midiOutput.open("LoopBe Internal MIDI");

}

Mat FormKeyer::process(const Mat &input){

    if(!connectedMIDI) {
        start();
        connectedMIDI = true;
    }

    // nur alle 1000 Mal ausführen -> eventuell bessere Option? Testen!!
    if (true) {


        Mat hsvImage, redMask, greenMask, blueMask,lowerRedHueRange, upperRedHueRange;

        // RGB to HSV
        cvtColor(input, hsvImage, CV_BGR2HSV);


        // Nach Rot suchen - oberer und unterer Bereich der 360 Grad
        inRange(hsvImage, Scalar(redHueLower1, 100, 100), Scalar(redHuerUpper1, 255, 255), lowerRedHueRange);
        inRange(hsvImage, Scalar(redHueLower2, 100, 100), Scalar(redHuerUpper2, 255, 255), upperRedHueRange);
        // Verunden der beiden Rotbereiche
        bitwise_or(lowerRedHueRange,upperRedHueRange, redMask);

        // Nach Grün suchen
        inRange(hsvImage, Scalar(greenHueLower, greenSatLower, greenLigLower), Scalar(greenHueUpper, greenSatUpper, greenLigUpper), greenMask);
        // Nach Blau suchen
        inRange(hsvImage, Scalar(blueHueLower, blueSatLower, blueLigLower), Scalar(blueHueUpper, blueSatUpper, blueLigUpper), blueMask);

        // entfernt kleine Störungen
        erode(greenMask, greenMask, Mat());
        //schließt die Objekte
        dilate(greenMask,greenMask,Mat());
        morphologyEx(greenMask,greenMask, MORPH_CLOSE, 10);

        // verwischt die Kanten -> austesten Sinn?
        //GaussianBlur(greenMask,greenMask,Size(3,3),0);


        Mat output = detectForms(greenMask, input);
        counter = 0;
        midiOutput.sendNoteOn(1, 60, 127);


        return output;
    } else {
        counter += 1;
        return input;
    }

}


Mat FormKeyer::detectForms(const Mat &mask, const Mat &input) {


    // Speichern der Kontur
    vector<vector<Point> > contours;

    // Funktion zum Finden von Konturen
    // CV_RETR_LIST -> gibt eine Liste aller Konturen zurück ohne diese hierarchisch anzuordnen
    // CV_CHAIN_APPROX_SIMPLE -> vereinfacht die Kontur
    // Point(0,0) -> offset der jeden Punkt einer Kontur shifted
    findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // Speicher für approximierte Form
    vector<Point> approxPoly;
    int form;
    int x, y;
    vector<Moments> mu(contours.size());
    vector<Point2f> mc(contours.size());

    // Schleife durch alle gefundenen Konturen
    for(size_t i = 0; i < contours.size(); i++){

        // Approximiert ein Polygon mit weniger Vertices mittels Ramer-Douglas-Peucker-Algorithmus (Algorithmus zur Kurvenglättung)
        // contours[i] -> input, approxPoly -> output
        // arcLength(..) -> epsilon -> legt die genauigkeit der Approximierung fest -> arcLength -> kalkuliert den Umfang der Kontur, true für geschlossene Kontur
        // true für geschlossene Kontur
        approxPolyDP(contours[i], approxPoly, arcLength(Mat(contours[i]), true)*0.05, true);

        // Schaut nach der Anzahl der Vertices -> wenn es drei sind, dann ist es ein Dreieck
        if(approxPoly.size() == 3){
            form = TRIANGLE_NUMBER;
            drawContours(input, contours, i, Scalar(0, 255, 0), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxPoly.begin(); vertex != approxPoly.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
        else if(approxPoly.size() == 4){
            form = QUADER_NUMBER;
            drawContours(input, contours, i, Scalar(0, 0, 255), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxPoly.begin(); vertex != approxPoly.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }
        else if(approxPoly.size() == 5){
            form = PENTAGON_NUMBER;
            drawContours(input, contours, i, Scalar(255, 0, 0), 3); // fill GREEN
            vector<Point>::iterator vertex;
            for(vertex = approxPoly.begin(); vertex != approxPoly.end(); ++vertex){
                circle(input, *vertex, 3, Scalar(0, 0, 255), 1);
            }
        }

        // Vielleicht eher hiermit arbeiten: https://stackoverrun.com/de/q/2904651 -> boundingRect, weniger Aufwand
        mu[i] = moments( contours[i], false );
        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        circle( input, mc[i], 4, Scalar(0,0,0), -1, 8, 0 );
    }

    return input;
}


