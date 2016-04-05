#include "../headers/Brain.hpp"

Brain::Brain(int fps, Mat frame, int nthreads){
	this->fps = fps;
	this->frame = frame;
	this->nthreads = nthreads;

	//starta o counter
	this->counter = Counter(nthreads);
	//start os trackBlocks
	for(int i = 0; i < nthreads; i++){
		blocks.push_back(new TrackBlock(&counter, &blocker));
	}
	cout << "local" << endl;
}

void Brain::run(int){
	blocker.run();
}
void Brain::reduce(){
	counter.waitMax();
}
void Brain::refactore(ullint){}

void Brain::buildOutput(){
	for(uint i = 0; i < tracklets.size();i++){
		tracklets[i].draw();
	}
}

int Brain::numberObjects(){
	return 0;
}
Point Brain::objectPosic(int){}
void Brain::saveImage(){}

void Brain::init(){
	for(uint i = 0; i < tracklets.size();i++){
		blocks[i%nthreads]->addTrackler(&tracklets[i]);
	}

	for(uint i = 0; i < blocks.size();i++)
		blocks[i]->start();


}
