#ifndef TRACK_H
#define TRACK_H

#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <cstdlib>

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

	Point realwin, realsize;


	//debug
	bool debug;
	bool adaptable;
	double winProp;

	Mat templ;
	Mat frame;
	Mat mascara;
	//kalman variables

public:
	Tracking(double, Mat mainImage, Point, Point, double , bool, bool);
	void process(Mat nextImage);
	Point mymatchTemplate();
};

#endif
