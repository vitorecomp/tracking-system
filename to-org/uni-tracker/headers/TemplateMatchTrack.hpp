#ifndef TEMPL_MATCH_TRACKER
#define TEMPL_MATCH_TRACKER

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

#include "baseTracker.hpp"

class TemplateMatchTracker : public Tracker{
	Point actual_position;
	Point size, distance;
	Point realwin, realsize;

	Mat templ, frame;

	Point leftUp;
	Point winSize;
	Point shiftUp;


	bool adaptable;
	uint windowProp;

	Point mymatchTemplate();
public:
	Point getPosition();
	void track(Mat);
	void setTarget(Mat, Point, Point);
	void reconfigure(map<string, string>);
};

#endif
