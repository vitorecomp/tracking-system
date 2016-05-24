#include "Tracker.hpp"

Mat Tracker::getFile(string name){
	Mat img = imread(name.c_str());
	if(!img.data)
		throw invalid_argument("Image name invalid");
	return img;
}

Mat Tracker::getTempl(Mat frame, Point center, Point size){
	Mat templ;
	try{
		templ = Mat(frame, Rect(center.x - size.x/2, center.y - size.y/2, size.x, size.y));
	}catch(...){
		throw invalid_argument("Invalid args to create a template");
	}
	return templ;
}
