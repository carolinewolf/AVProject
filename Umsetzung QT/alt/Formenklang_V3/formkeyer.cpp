#include "formkeyer.h"
#include <QDebug>
#include <vector>
#include "form.h"
#include <QTextStream>


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

const int GREEN_FORM = 1;
const int BLUE_FORM = 2;
const int RED_FORM = 3;

QTextStream out(stdout);


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
    if (counter > 50) {

        cout << "--------------------------------" << endl;
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


        vector<vector<int>> greenForms = detectForms(greenMask, GREEN_FORM);
        vector<vector<int>> redForms = detectForms(redMask, RED_FORM);
        vector<vector<int>> blueForms = detectForms(blueMask, BLUE_FORM);
        vector<vector<int>> allForms(greenForms.size() + redForms.size() + blueForms.size());

        allForms.reserve( greenForms.size() + redForms.size() + blueForms.size() );
        allForms.insert( allForms.end(), greenForms.begin(), greenForms.end() );
        allForms.insert( allForms.end(), redForms.begin(), redForms.end() );
        allForms.insert( allForms.end(), blueForms.begin(), blueForms.end());

    if(allForms.size() < 0) {
        for(size_t i = 0; i < allForms.size(); i ++) {
            midiOutput.sendNoteOn(allForms[i][0], allForms[i][1], allForms[i][2]);
        }
    }
        counter = 0;

        return input;
    } else {
        counter += 1;
        return input;
    }

}


vector<vector<int>> FormKeyer::detectForms(const Mat &mask, const int color) {


    // Speichern der Kontur
    vector<vector<Point> > contours;

    // Funktion zum Finden von Konturen
    // CV_RETR_LIST -> gibt eine Liste aller Konturen zurück ohne diese hierarchisch anzuordnen
    // CV_CHAIN_APPROX_SIMPLE -> vereinfacht die Kontur
    // Point(0,0) -> offset der jeden Punkt einer Kontur shifted
    findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    // Speicher für approximierte Form
    vector<Point> approxPoly;
    int formType;
    int x, y;
    vector<vector<int>> forms (contours.size());

    // Schleife durch alle gefundenen Konturen
    for(size_t i = 0; i < contours.size(); i++){


        if(contours[i].size() > 20){
        // Approximiert ein Polygon mit weniger Vertices mittels Ramer-Douglas-Peucker-Algorithmus (Algorithmus zur Kurvenglättung)
        // contours[i] -> input, approxPoly -> output
        // arcLength(..) -> epsilon -> legt die genauigkeit der Approximierung fest -> arcLength -> kalkuliert den Umfang der Kontur, true für geschlossene Kontur
        // true für geschlossene Kontur
        approxPolyDP(contours[i], approxPoly, arcLength(Mat(contours[i]), true)*0.05, true);

        Rect bound = boundingRect(Mat(contours[i]));
        x = bound.x + (bound.width/2);
        y = bound.y + (bound.height/2);

        // Schaut nach der Anzahl der Vertices -> wenn es drei sind, dann ist es ein Dreieck
        switch(approxPoly.size()) {
            case 3 : formType = TRIANGLE_NUMBER;
            out << "----------" << endl;
            out << "Dreieck" << endl;
            out << color << endl;
            out <<  contours[i].size() << endl;
            out << bound.width << endl;
            out << bound.height << endl;
                     break;
            case 4 : formType = QUADER_NUMBER;
            out << "----------" << endl;
            out << "Quader" << endl;
            out << color << endl;
            out <<  contours[i].size() << endl;
            out << bound.width << endl;
            out << bound.height << endl;

                     break;
            case 5 : formType = PENTAGON_NUMBER;
            out << "----------" << endl;
            out << "Pentagon" << endl;
            out << color << endl;
            out <<  contours[i].size() << endl;
            out << bound.width << endl;
            out << bound.height << endl;

                     break;
            default:
                     break;
        }

        // kalkuliert ein Rechteck um das Punkte-Set -> dadurch kann der Mittelpunkt berechnet werden


         vector<int> form = {formType, x, y};
         forms[i] = form;
         }

    }


    return forms;
}


