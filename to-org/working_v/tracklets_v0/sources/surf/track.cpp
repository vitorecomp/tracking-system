#include "track.h"

Tracking::Tracking(double fps, Mat mainImage, Point center, Point size){
    //pega a imagem

    leftUp = Point(center.x - size.x/2, center.y - size.y/2);

	this->templ = Mat(mainImage, Rect(leftUp.x, leftUp.y, size.x, size.y)).clone();


    Mat featureImage;
    // Construct the SURF feature detector object
    this->hold = io::uniArgs.suft_threshold;
    cv::SurfFeatureDetector surf(hold); // threshold
    // Detect the SURF features
    surf.detect(templ,keypoints);

    cv::drawKeypoints(templ, keypoints, featureImage, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    surfDesc.compute(templ,keypoints,descriptors);
    imshow("ImagemL", featureImage);
    waitKey(50);

    //define center pading of the center off points
}


void Tracking::process(Mat frame){
    Mat featureImage;
    vector<cv::KeyPoint> keypoints2;
	Mat descriptors2;
    SurfFeatureDetector surf(hold); // threshold

    // Detect the SURF features
    surf.detect(frame,keypoints2);
    cv::drawKeypoints(frame,
            keypoints2,
            featureImage,
            cv::Scalar(255,255,255),
            cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    surfDesc.compute(frame,keypoints2,descriptors2);
    imshow("ImagemN", featureImage);
    waitKey(50);

    cv::BruteForceMatcher<cv::L2<float> > matcher;
    // Match the two image descriptors
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors,descriptors2, matches);

    cv::Mat imageMatches;
    cv::drawMatches(
        templ,keypoints,    // 1st image and its keypoints
        frame,keypoints2,   // 2nd image and its keypoints
        matches,            // the matches
        imageMatches,       // the image produced
        cv::Scalar(255,255,255)); // color of the lines
    imshow("ImagemFinal", imageMatches);
    waitKey(50);
}
