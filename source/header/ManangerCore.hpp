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

//files
#include "./IOMananger.hpp"
#include "./Tracklet.hpp"

//define

typedef struct timeval time_us;
typedef unsigned long long int ulint;

//namespaces
using namespace std;

class ManangerCore{

    enum METHOD {TEMPLATE_MACTHING, SIFT, SURF, BACKGROUND_SUBTRACTION};
    METHOD method;

    enum METRIC {ACURACY, TIME};
    METRIC metric;

    enum DIST_METHOD {DYNAMIC, STATIC};
    DIST_METHOD dist_method;

    vector<Tracklet> tracklets;

    METRIC convertMetric(string);
    METHOD convertMethod(string);
    METRIC convertMetric(string);

    void defineMetric();
    void defineDistMethod();
    void defineStartTrackelet();

public:

    void startTrackelets();
    void run();
    void buildOutputEnv();
    void generateInformation();
};

#endif
