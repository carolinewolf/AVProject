#include "formkeyer.h"
#include <QDebug>
#include <vector>
using namespace cv;
using namespace std;
FormKeyer::FormKeyer()
{

}


Mat FormKeyer::process(const Mat &input){
    Mat imageGreyScale;
    cvtColor(input, imageGreyScale, CV_BGR2GRAY);
    threshold(imageGreyScale,imageGreyScale,128,255,CV_THRESH_BINARY);



    return imageGreyScale;

}
