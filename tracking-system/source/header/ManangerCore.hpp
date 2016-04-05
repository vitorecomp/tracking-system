#ifndef MANANGER_CORE_H
#define MANANGER_CORE_H

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

class ManangerCore{

    enum METHOD {TEMPLATE_MACTHING, SIFT, SURF, BACKGROUND_SUBTRACTION};
    METHOD method;

    enum METRIC {ACURACY, TIME};
    METRIC metric;

    enum DIST_METHOD {DYNAMIC, STATIC};
    DIST_METHOD dist_method;

    void makeTracklets(uint);


    vector<Tracklet> tracklets;
    vector<TrackletBlock> blocks;

    UniversalQueue universal_queue;

    METRIC convertMetric(string);
    METHOD convertMethod(string);
    DIST_METHOD convertDistMethod(string);

    void defineMetric();
    void defineDistMethod();
    void defineStartTrackelet();

    void startUniversalQueue();

    //report
    void sumirizeTimeReport();
    void sumirizeErrorReport();
    void sumirizeChangeReport();
    void sumirizeFullReport();

    //input
    std::vector<string> frames;
    Mat getFrame();
    void getFrames(string);
    void getTargetsPosition(string arqui, uint frame);

    //output
    string outputs;
    void createFolders();

    //execution
    void redefineMethods();
    void evalResults();
    void executeAll();

    void redistributeTargets(vector<Target>);
    vector<Target> getNewPositons();

	//video File
	VideoCapture cap;

public:

    void startTrackelets();
    void run();
    void buildOutputEnv();
    void generateInformation();

};

#endif
