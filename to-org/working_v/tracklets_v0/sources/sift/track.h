#ifndef TRACK_H
#define TRACK_H

#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace cv;
using namespace std;

class Tracking
{
private:
	double fps;
	//kalman positon
	pthread_mutex_t clicks;
  	bool hasclicks;
	int nclicks;

	Point leftUp, rightDown;
	Point size;

	Mat templ;
	double hold;
	Mat iframe;
	std::vector<cv::KeyPoint> keypoints;
	// Construction of the SURF descriptor extractor
	cv::SurfDescriptorExtractor surfDesc;
	// Extraction of the SURF descriptors
	cv::Mat descriptors;

public:
	Tracking(Mat mainImage);
	void process(Mat nextImage);

	bool twoCLicks();
  	void click(int, int);
};
void mouseHandler(int event, int x, int y, int flags, void *param);


#endif
