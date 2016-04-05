#ifndef SIFT_TRACKER
#define SIFT_TRACKER


#include "Tracker.hpp"

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
