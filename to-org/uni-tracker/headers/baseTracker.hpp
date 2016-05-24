#ifndef BASE_TRACKER
#define BASE_TRACKER

#include <vector>
#include <cstdlib>
#include <cstdio>

#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include <sys/time.h>

using namespace cv;
using namespace std;

class Tracker{


public:
	virtual Point getPosition() = 0;
	virtual void track(Mat)  = 0;
	virtual void setTarget(Mat, Point, Point) = 0;
	virtual void reconfigure(map<string, string>) = 0;

	static Mat getTempl(Mat, Point, Point);
	static Mat getFile(string);

};


#endif
