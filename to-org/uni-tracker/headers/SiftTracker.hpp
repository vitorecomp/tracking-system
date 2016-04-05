#ifndef SIFT_TRACKER
#define SIFT_TRACKER

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

#include <sys/time.h>

using namespace cv;
using namespace std;

#include "baseTracker.hpp"

class SiftTracker : public Tracker{
	Point actual_position;
	Point size, distance;
	Mat templ;


	std::vector<cv::KeyPoint> keypoints;
	cv::SurfDescriptorExtractor surfDesc;
	cv::Mat descriptors;
	uint hold;
public:
	Point getPosition();
	void track(Mat);
	void setTarget(Mat, Point, Point);
	void reconfigure(map<string, string>);
};

#endif
