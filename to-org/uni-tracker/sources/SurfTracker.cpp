#include "../headers/SurfTracker.hpp"

SurfTracker::SurfTracker(){

}

Point SurfTracker::getPosition(){
	return actual_position;
}


void SurfTracker::setTarget(Mat templ, Point actual_center, Point size){
    this->templ = templ.clone();
    this->actual_position = actual_center;
    this->size = size;

    Mat featureImage;
    // Construct the SURF feature detector object
    while(1){
        cv::SurfFeatureDetector surf(hold--); // threshold
        // Detect the SURF features
        surf.detect(templ,keypoints);
        if(keypoints.size() > 3)
            break;
        if(hold == 0)
            break;
    }

    double x = 0, y = 0, size_v = keypoints.size();
    for(unsigned int i = 0; i < keypoints.size(); i++){
        x += keypoints[i].pt.x;
        y += keypoints[i].pt.y;
    }

    Point center_v(x/size_v, y/size_v);
    center_v = center_v + actual_center - Point(size.x/2, size.y/2);
    distance = center_v - actual_center;

    cv::drawKeypoints(templ, keypoints, featureImage, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    surfDesc.compute(templ,keypoints,descriptors);
}

void SurfTracker::reconfigure(map<string, string> args){
	std::map<string,string>::iterator it;
	it = args.find("sift_thrshold");
  	if (it != args.end())
		hold = stoi(args["sift_thrshold"]);
	else
		hold = 100;
}

void SurfTracker::track(Mat frame){

    //BaseSetup
	Mat featureImage;
    vector<cv::KeyPoint> keypoints2;
	Mat descriptors2;
    SurfFeatureDetector surf(hold);

    // Detect the SURF features
    surf.detect(frame,keypoints2);
    cv::drawKeypoints(frame,
            keypoints2,
            featureImage,
            cv::Scalar(255,255,255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    surfDesc.compute(frame,keypoints2,descriptors2);

    // Match the two image descriptors
    cv::BruteForceMatcher<cv::L2<float> > matcher;
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors,descriptors2, matches);

    //fazendo a media
	double x = 0, y = 0, size_v = matches.size();
	for(unsigned int i = 0; i < matches.size(); i++){

		x += keypoints2[matches[i].trainIdx].pt.x;
		y += keypoints2[matches[i].trainIdx].pt.y;
	}
    if(size_v != 0)
	   actual_position = Point(x/size_v, y/size_v);

    cv::Mat imageMatches;

    cv::drawMatches(
        templ,keypoints,    // 1st image and its keypoints
        frame,keypoints2,   // 2nd image and its keypoints
        matches,            // the matches
        imageMatches,       // the image produced
        cv::Scalar(255,255,255)); // color of the lines
}
