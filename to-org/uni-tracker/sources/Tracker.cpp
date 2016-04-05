#include "../headers/Tracker.hpp"


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

TrackBlock::TrackBlock(Counter *counter, Blocker *blocker){
	this->counter = counter;
	this->blocker = blocker;
	work = 0;
}

void TrackBlock::run(){
	work = blocker->waitRunSignal(work);
	for(uint i = 0; i < tracks.size(); i++){
		tracks[i]->process();
	}
	counter->increment();
}

void TrackBlock::addTrackler(Tracklet *value){
	this->tracks.push_back(value);
}

void TrackBlock::start(){
	runSignal();
}

Tracklet::Tracklet(Mat frame, Point center, Point size){

	this->type = 0;
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

	Mat templ = Mat(frame, Rect(this->leftUp.x, this->leftUp.y, this->size.x, size.y));
	this->templ = templ.clone();
	//this->tracker = (Tracker*) new SurfTracker();
	//this->tracker = (Tracker*) new TemplateMatchTracker();

	this->tracker = (Tracker*) new SurfTracker();
	this->tipo = 3;

	tracker->reconfigure(io::uni_args.map_args);
	tracker->setTarget(templ, center, size);
}

void Tracklet::reduce(){
	if(tipo == 1)
		return;

	tipo--;

	int inter = center.x - size.x/2;
	this->leftUp.x = inter < 0 ? 0 : inter;
	inter = center.y - size.y/2;
	this->leftUp.y = inter < 0 ? 0 : inter;

	inter = center.x + size.x/2;
	this->riDown.x = inter > frame.cols ? frame.cols-1 : inter;
	inter = center.y + size.y/2;
	this->riDown.y = inter > frame.rows ? frame.rows-1 : inter;

	Mat templ = Mat(frame, Rect(this->leftUp.x, this->leftUp.y, this->size.x, size.y));
	this->templ = templ.clone();

	if(tipo == 2){
		free(this->tracker);
		this->tracker = (Tracker*) new TemplateMatchTracker();
	}

	if(tipo == 1){
		free(this->tracker);
		this->tracker = (Tracker*) new SiftTracker();
	}

	tracker->reconfigure(io::uni_args.map_args);
	tracker->setTarget(templ, center, size);
}

void Tracklet::upper(){
	if(tipo == 3)
		return;

	tipo++;

	int inter = center.x - size.x/2;
	this->leftUp.x = inter < 0 ? 0 : inter;
	inter = center.y - size.y/2;
	this->leftUp.y = inter < 0 ? 0 : inter;

	inter = center.x + size.x/2;
	this->riDown.x = inter > frame.cols ? frame.cols-1 : inter;
	inter = center.y + size.y/2;
	this->riDown.y = inter > frame.rows ? frame.rows-1 : inter;

	Mat templ = Mat(frame, Rect(this->leftUp.x, this->leftUp.y, this->size.x, size.y));
	this->templ = templ.clone();

	if(tipo == 2){
		free(this->tracker);
		this->tracker = (Tracker*) new TemplateMatchTracker();
	}

	if(tipo == 3){
		free(this->tracker);
		this->tracker = (Tracker*) new SurfTracker();
	}

	tracker->reconfigure(io::uni_args.map_args);
	tracker->setTarget(templ, center, size);
}

void Tracklet::process(){
	time_us start;
	time_us end;

	gettimeofday(&start, NULL);
	tracker->track(frame);
	gettimeofday(&end, NULL);

	uint tempo = Time::time_to_int(start, end);
	#ifdef debug
		cout << tempo << endl;
	#endif


	tempos.push_back(tempo);
	tipos.push_back(tipo);

	if(tempo > LOCAL_HOLD){
		this->reduce();
		return;
	}

	if(tempo + DIFF < LOCAL_HOLD)
		this->upper();
}

void Tracklet::draw(){
	leftUp =  tracker->getPosition() - Point(size.x/2, size.y/2);
	#ifdef debug
		riDown = leftUp + size;
		cout << leftUp << endl;
		cout << riDown << endl;
	#endif
	rectangle(frame, leftUp, riDown, Scalar( 0, 255, 255 ), 1, 8, 0);
}

void Tracklet::saveGrapth(){
	static int num  = 0;
	std::fstream fs;

	stringstream ss;
	ss << "uotput" << num++ << ".csv";
	fs.open (ss.str().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
	for(uint j =0; j < 3; j++){
		for(uint i = 0; i < tempos.size();i++){
			if(tipos[i] == j)
				fs << 1 << ";";
			else
				fs << 0 << ";";
		}
		fs << endl;
	}
	fs.close();


	Mat image = Mat::zeros(400, tempos.size()*12, CV_8UC3);
	for(uint i = 0; i < tempos.size();i++){
		if(tipos[i] == 1)
			line(image, Point(i*12, 400 - round(tempos[i]/10000)), Point(i*12+11, 400-round(tempos[i]/10000)), Scalar(255, 0, 0));
		if(tipos[i] == 2)
			line(image, Point(i*12, 400 - round(tempos[i]/10000)), Point(i*12+11, 400-round(tempos[i]/10000)), Scalar(0, 255, 0));
		if(tipos[i] == 3)
			line(image, Point(i*12, 400 - round(tempos[i]/10000)), Point(i*12+11, 400-round(tempos[i]/10000)), Scalar(0, 0, 255));
	}
	imshow("grapf", image);
	waitKey(50);
}
