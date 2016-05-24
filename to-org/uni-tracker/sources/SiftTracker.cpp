#include "../headers/SiftTracker.hpp"

void SiftTracker::setTarget(Mat templ, Point actual_center, Point size){
	this->templ = templ.clone();
	this->actual_position = actual_center;
	this->size = size;

	Mat featureImage;
    // Construct the SURF feature detector object
    cv::SiftFeatureDetector sift; // threshold
    // Detect the SURF features
    sift.detect(templ,keypoints);
	double x = 0, y = 0, size_v = keypoints.size();
	for(int i = 0; i < keypoints.size(); i++){
		x += keypoints[i].pt.x;
		y += keypoints[i].pt.y;
	}
	Point center_v(x/size_v, y/size_v);
	center_v = center_v + actual_center - Point(size.x/2, size.y/2);
	distance = center_v - actual_center;

    cv::drawKeypoints(templ, keypoints, featureImage, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    surfDesc.compute(templ,keypoints,descriptors);
	#ifdef debug
	//imshow("ImagemL", featureImage);
	#endif
}

void SiftTracker::reconfigure(map<string, string> args){
	std::map<string,string>::iterator it;
	it = args.find("sift_thrshold");
  	if (it != args.end())
		hold = stoi(args["sift_thrshold"]);
	else
		hold = 100;
}


void SiftTracker::track(Mat frame){
	Mat featureImage;
    vector<cv::KeyPoint> keypoints2;
	Mat descriptors2;
    SiftFeatureDetector sift(0); // threshold

    // Detect the SURF features
    sift.detect(frame,keypoints2);
    cv::drawKeypoints(frame,
            keypoints2,
            featureImage,
            cv::Scalar(255,255,255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    surfDesc.compute(frame,keypoints2,descriptors2);

    cv::BruteForceMatcher<cv::L2<float> > matcher;
    // Match the two image descriptors
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors,descriptors2, matches);

	double x = 0, y = 0, size_v = matches.size();
	for(int i = 0; i < matches.size(); i++){
		x += keypoints2[matches[i].trainIdx].pt.x;
		y += keypoints2[matches[i].trainIdx].pt.y;
	}

	actual_position = Point(x/size_v, y/size_v) - distance;

    cv::Mat imageMatches;
    cv::drawMatches(
        templ,keypoints,    // 1st image and its keypoints
        frame,keypoints2,   // 2nd image and its keypoints
        matches,            // the matches
        imageMatches,       // the image produced
        cv::Scalar(255,255,255)); // color of the lines
	//imshow("Templ", templ);
    //imshow("ImagemN", frame);
    //imshow("ImagemFinal", imageMatches);
}

Point SiftTracker::getPosition(){
	return actual_position;
}
