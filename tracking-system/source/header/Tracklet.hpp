#ifndef TRACKLETS
#define TRACKLETS

//includes
#include <thread>
#include <mutex>
#include <iostream>
#include <unistd.h>
#include <sstream>
#include <cmath>
#include <vector>
#include <sys/time.h>
#include <algorithm>
#include <highgui.h>
#include <cv.h>

//files
#include "../libs/output/IOMananger.hpp"
#include "./Tracklet.hpp"
#include "./queue/queue.hpp"

//define

typedef struct timeval time_us;
typedef unsigned long long int ulint;

//namespaces
using namespace std;
using namespace cv;

//class

class Target{
public:
	Point posic;
	uint id;
};

class Tracklet : public Target{
public:
    enum METHOD {TEMPLATE_MACTHING, SIFT, SURF, BACKGROUND_SUBTRACTION};

	METHOD method;
    void setMethod(METHOD);
	void startMethod();

    void setFrame(Mat);

    void saveGraph(string);
    void saveTimes(string);
};

class TrackletBlock{
	std::vector<Tracklet> tracklets;
	UniversalQueue *queue;
public:
	enum METRIC {ACURACY, TIME};

	METRIC metric;
	void setMetric(METRIC);

	void addTarget(Tracklet&);
	void addQueue(UniversalQueue*);

	void setFrame(Mat);

	void saveGraph(string);
	void saveTimes(string);

};

#endif
