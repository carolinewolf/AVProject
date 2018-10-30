INCLUDEPATH += ../opencv
HEADERS += ../opencv/cvmattoqimage.h

win32 {
#  fuer Windows muss die Umgebungsvariable OPENCV_DIR gesetzt sein
#  auf den Pfad der OpenCV-Installation
#  Beispiel: C:\OpenCV-MinGW-Build-OpenCV-3.4.1

    INCLUDEPATH += $$(OPENCV_DIR)\include
    LIBS += $$(OPENCV_DIR)\bin\libopencv_*.dll
}
macx {
# seit WS1617 nicht getestet
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
    LIBS +=  -lopencv_core -lopencv_highgui -lopencv_imgproc
}
linux {
# seit WS1617 nicht getestet
    INCLUDEPATH += /usr/local/include
    LIBS += -L/usr/local/lib
    LIBS +=  -lopencv_core -lopencv_highgui -lopencv_imgproc
}
