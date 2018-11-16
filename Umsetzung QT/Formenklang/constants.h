#ifndef CONSTANTS_H
#define CONSTANTS_H


// --- Red
// Hue
const double redHueLower1 = 0;
const double redHuerUpper1 = 20 / 2;
const double redHueLower2 = 340 / 2;
const double redHuerUpper2 = 360 / 2;
// Satturation
const double redSatLower = (55 * 2.55);
const double redSatUpper = (100 * 2.55);
// Light
const double redLigLower = (15 * 2.55);
const double redLigUpper = (100 * 2.55);


// --- Green
// Hue
const double greenHueLower = 120 / 2 - 20;
const double greenHueUpper = 120 / 2 + 20;
// Satturation
const double greenSatLower = (30 * 2.55);
const double greenSatUpper = (100 * 2.55);
// Light
const double greenLigLower = (15 * 2.55);
const double greenLigUpper = (100 * 2.55);


// --- Blue
// Hue
const double blueHueLower = 200 / 2;
const double blueHueUpper = 300 / 2;
//Satturation
const double blueSatLower = (30 * 2.55);
const double blueSatUpper = (100 * 2.55);
// Light
const double blueLigLower = (15 * 2.55);
const double blueLigUpper = (100 * 2.55);


const int TRIANGLE_NUMBER = 10;
const int QUADER_NUMBER = 20;
const int PENTAGON_NUMBER = 30;

const int GREEN_FORM = 1;
const int BLUE_FORM = 2;
const int RED_FORM = 3;



#endif // CONSTANTS_H
