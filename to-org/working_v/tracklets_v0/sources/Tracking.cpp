#include "../headers/Tracking.hpp"
#define MET2

Tracklet::Tracklet(Mat frame, Point center, Point size){
	this->frame = frame;
	this->center = center;
	this->size = size;

	int inter = center.x - size.x/2;
	this->leftUp.x = inter < 0 ? 0 : inter;
	inter = center.y - size.y/2;
	this->leftUp.y = inter < 0 ? 0 : inter;

	inter = center.x + size.x/2;
	this->riDown.x = inter > frame.cols ? frame.cols-1 : inter;
	inter = center.y + size.y/2;
	this->riDown.y = inter > frame.rows ? frame.rows-1 : inter;

	Mat templ = Mat(frame, Rect(leftUp.x, leftUp.y, size.x, size.y));
	this->templ = templ.clone();

	this->track = Tracking(30, frame.clone(), center, size, 3, false, false);
	this->track2 = FullTMSequencial(30, frame.clone(), center, size, 3, false, false);
}

void Tracklet::process(){
	#ifdef MET1
		track.process(frame);
		this->leftUp = track.sLeftUp;
		this->riDown = track.sridown;
	#endif
	#ifdef MET2
		track2.process(frame);
		this->leftUp = track2.sLeftUp;
		this->riDown = track2.sridown;
	#endif
	std::cout << "Rodei" << std::endl;
}

void Tracklet::draw(){
	rectangle(frame, leftUp, riDown, Scalar( 0, 255, 255 ), 1, 8, 0);
}

TrackBlock::TrackBlock(Counter *counter, Blocker *blocker){
	this->counter = counter;
	this->blocker = blocker;
	work = 0;
}

void TrackBlock::run(){
	work = blocker->waitRunSignal(work);
	for(uint i = 0; i < tracks.size(); i++)
		tracks[i]->process();
	counter->increment();
}

void TrackBlock::addTrackler(Tracklet *value){
	this->tracks.push_back(value);
}

void TrackBlock::start(){
	runSignal();
}
