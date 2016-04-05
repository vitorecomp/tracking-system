#ifndef QUEUE_H
#define QUEUE_H

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
#include "../../libs/output/IOMananger.hpp"

//define

typedef struct timeval time_us;
typedef unsigned long long int ulint;

//namespaces
using namespace std;
using namespace cv;

//class

class UniversalQueue{
public:
	void start();
};

#endif
