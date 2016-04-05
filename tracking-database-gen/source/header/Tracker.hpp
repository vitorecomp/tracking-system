#ifndef TRACKER_H
#define TRACKER_H

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

//define

typedef struct timeval time_us;
typedef unsigned long long int ulint;

//namespaces
using namespace std;
using namespace cv;

class Tracker{
private:

	static uint to_id;

	bool dead;
	Mat image;

	uint red, blue, green;
	uint size_x, size_y;
	uint max_x_pos, max_y_pos;
	uint x_position, y_position;

	Mat frames;

	void setPosition();
	void createColors();
	void createMat();
	void recreate();

	Scalar randomColor();

public:

	vector<Point> positions;
	uint id;

	Tracker(Mat);
	void generatePosition();
	void draw();
};

class TrackerCore{
	uint x_res;
  	uint y_res;

  	Mat frame;

	uint num_trackers;
	uint num_frames;

	std::vector<Tracker> trackers;

public:
    TrackerCore();

    Size getVideoSize();
    uint getFrameRate();
    string getVideoName();

    void startTrackers();

    bool hasFrames();

    void generatePositions();
    void draw();
    Mat getFrame();

	void generateGroundTruth();
    void saveDatabaseJson();
};

#endif
