#include "../header/Tracklet.hpp"

void Tracklet::setMethod(Tracklet::METHOD method){
	this->method = method;
	this->startMethod();
}

void Tracklet::startMethod(){

}

void TrackletBlock::setMetric(TrackletBlock::METRIC metric){
	this->metric = metric;
}

void TrackletBlock::addTarget(Tracklet &tracklet){
	tracklets.push_back(tracklet);
}

void TrackletBlock::addQueue(UniversalQueue *queue){
	this->queue = queue;
}
