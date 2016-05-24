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

//define

typedef struct timeval time_us;
typedef unsigned long long int ulint;

//namespaces
using namespace std;
using namespace cv;

//class

class UniversalQueue{

};

class Target{

};

class Tracklet{
public:
    enum METRIC {ACURACY, TIME};
    enum METHOD {TEMPLATE_MACTHING, SIFT, SURF, BACKGROUND_SUBTRACTION};

    void setMetric(METRIC);
    void setMethod(METHOD);

    void addTarget(Target&);
    void addQueue(UniversalQueue*);

    void setFrame(Mat);

    void saveGraph(string);
    void saveTimes(string);
};

#endif
