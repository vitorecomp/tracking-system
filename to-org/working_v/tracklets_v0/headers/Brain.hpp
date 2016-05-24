#ifndef BRAIN_H
#define BRAIN_H

#include "Tracking.hpp"

class Brain{
	int fps;
	Mat frame;
	int nthreads;

	Counter counter;
	Blocker blocker;
public:
	vector<Tracklet> tracklets;
	vector<TrackBlock*> blocks;



	Brain(int, Mat, int);

	void init();
	void run(int);
	void reduce();
	void refactore(ullint);
	void buildOutput();

	int numberObjects();
	Point objectPosic(int);

	void saveImage();
};

#endif
