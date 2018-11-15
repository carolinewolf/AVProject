# Formenklang

Projektaufgabe
Wählen Sie eine der folgenden drei Alternativen:
1. Erstellen Sie mit C++ eine Videosoftware, die mit Live-Videos arbeitet.
2. Erstellen Sie eine Software, die mit Hilfe künstlicher Intelligenz Musik erzeugt.
3. Erstellen Sie eine Videosoftware, die Musik steuert.

Ideen:
- Es können Formen auf einer Oberfläche positioniert werden.
- In C++ mit QT und opencv werden Formen und Farben getrackt.
- Jede Form-Farb Kombination steht für einen Sound.
- Per MIDI werden Werte der Form-Farb Kombination an die Web-Audio API gesendet.
- Der Browser spielt dann die passenden Töne ab.
- Dieser Sound kann im Browser durch Regler bearbeitet und gedownloaded werden.

Hilfreiche Links:
https://medium.com/creative-technology-concepts-code/recording-syncing-and-exporting-web-audio-1e1a1e35ef08
https://stackoverflow.com/questions/16969201/web-audio-api-scheduling-sounds-and-exporting-the-mix
https://stackoverflow.com/questions/51076949/mix-wav-file-and-export-it-with-web-audio-api

https://blog.naver.com/monkey5255/220662430284
https://docs.opencv.org/3.3.1/d4/d73/tutorial_py_contours_begin.html



Dokumentation QT C++

findContours()
Definition: void findContours(InputOutputArray image, OutputArrayOfArrays contours, int mode, int method, Point offset=Point())// Funktion zum Finden von Konturen
Code:       findContours( mask, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

CV_RETR_LIST -> gibt eine Liste aller Konturen zurück ohne diese hierarchisch anzuordnen
CV_CHAIN_APPROX_SIMPLE -> vereinfacht die Kontur
Point(0,0) -> offset der jeden Punkt einer Kontur shifted

approxPolyDP
Code:approxPolyDP(contours[i], approxPoly, arcLength(Mat(contours[i]), true)*0.05, true);

Approximiert ein Polygon mit weniger Vertices mittels Ramer-Douglas-Peucker-Algorithmus (Algorithmus zur Kurvenglättung)
 contours[i] -> input, approxPoly -> output
arcLength(..) -> epsilon -> legt die genauigkeit der Approximierung fest -> arcLength -> kalkuliert den Umfang der Kontur, true für geschlossene Kontur
true für geschlossene Kontur

boundingRect
            // kalkuliert ein Rechteck um das Punkte-Set -> dadurch kann der Mittelpunkt berechnet werden



SAVED FOR LATER:
 cout << "----------" << endl;
cout << "DREIECK " << colorText << endl;
cout << "Breite: " << bound.width << " Hoehe: " << bound.height << " x: " << x << " y: " << y <<endl;



if (color == 1) {
        colorText = "Gruen";
    } else if (color == 2) {
        colorText = "Blau";
    } else if (color == 3) {
        colorText = "Rot";
    }