#ifndef TEMPLATE_MATCH_H
#define TEMPLATE_MATCH_H

#include <highgui.h>
#include <cv.h>
#include <iostream>

#include "BaseTypes.hpp"
#include "Thread.hpp"

using namespace cv;
using namespace std;

class FullTMSequencial
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
	Point sLeftUp, sridown;
	FullTMSequencial();
	FullTMSequencial(double, Mat mainImage, Point, Point, double , bool, bool);
	void process(Mat nextImage);
	Point mymatchTemplate();
};

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
	Point sLeftUp, sridown;
	Tracking();
	Tracking(double, Mat mainImage, Point, Point, double , bool, bool);
	void process(Mat nextImage);
	Point mymatchTemplate(Mat , Mat);
};

class Tracklet
{
private:
	//Alrith Position
	Point leftUp;
	Point riDown;
	Point center;
	Point size;

	Mat templ;
	Mat frame;
	Mat result;
	//kalman variables
	Tracking track;
	FullTMSequencial track2;
public:
	Tracklet(Mat, Point, Point);
	void process();
	void draw();
};

class TrackBlock : public Thread{
	Counter *counter;
	Blocker *blocker;
	vector<Tracklet*> tracks;
	int work;
public:
	TrackBlock();
	TrackBlock(Counter*, Blocker*);
	void addTrackler(Tracklet *);
	void run();
	void start();
};


#endif
