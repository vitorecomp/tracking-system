#ifndef SIFT_TRACKER
#define SIFT_TRACKER


#include "Tracker.hpp"

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
