#ifndef TRACKER_H
#define TRACKER_H

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

#include <fstream> 
#include <sys/time.h>

#include "./Thread.hpp"
#include "./Entities.hpp"
#include "./TemplateMatchTrack.hpp"
#include "./SiftTracker.hpp"
#include "./SurfTracker.hpp"

using namespace cv;
using namespace std;

typedef struct timeval time_us;
typedef unsigned long long int ulint;

#define DIFF 1000
#define LOCAL_HOLD 500000

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

	int type;

	uint tipo;
	//kalman variables
	Tracker *tracker;

	vector<uint> tempos;
	vector<uint> tipos;

public:
	void reduce();
	void upper();

	Tracklet(Mat, Point, Point);
	void process();
	void draw();

	void saveGrapth();
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
