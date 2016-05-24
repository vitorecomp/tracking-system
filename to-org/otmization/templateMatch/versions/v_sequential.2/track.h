#ifndef TRACK_H
#define TRACK_H

#include <highgui.h>
#include <cv.h>
#include <iostream>

using namespace cv;
using namespace std;

class Tracking
{
private:
	double fps;
	//kalman positon
	Point kalman;
	//Alrith Position
	Point leftUp;
	Point size;
	Point winSize;
	Point shiftUp;


	//debug
	bool debug;
	bool adaptable;
	int match_method;
	double winProp;

	Mat templ;
	Mat mascara;
	Mat result;
	//kalman variables

public:
	Tracking(double, Mat mainImage, Point, Point, double , bool, bool);
	void process(Mat nextImage);
	Point mymatchTemplate(Mat , Mat);
};

#endif
