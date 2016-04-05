#include "../header/Tracker.hpp"

TrackerCore::TrackerCore(){
	srand(time(NULL));


	this->x_res = io::configs.x_resolution;
	this->y_res = io::configs.y_resolution;

	if(io::configs.colored)
		this->frame = Mat::zeros(this->x_res, this->y_res, CV_8UC3);
	else
		this->frame = Mat::zeros(this->x_res, this->y_res, CV_8UC1);

	this->num_trackers = io::configs.num_trackers;

	this->num_frames = io::configs.video_frame_rate * io::configs.duration;
}


void TrackerCore::startTrackers(){
	for(uint i = 0; i < this->num_trackers; i++){
		trackers.push_back(Tracker(this->frame));
	}
}

std::string TrackerCore::getVideoName(){
	return io::configs.output_video_name;
}

uint TrackerCore::getFrameRate(){
	return io::configs.video_frame_rate;
}

Size TrackerCore::getVideoSize(){
	return Size(this->y_res, this->x_res);
}

void TrackerCore::generatePositions(){
	for(uint i = 0; i < this->num_trackers; i++){
		trackers[i].generatePosition();
	}
}
void TrackerCore::draw(){
	if(io::configs.colored)
		this->frame = Mat::zeros(this->x_res, this->y_res, CV_8UC3);
	else
		this->frame = Mat::zeros(this->x_res, this->y_res, CV_8UC1);

	for(uint i = 0; i < this->num_trackers; i++){
		trackers[i].draw();
	}
}

Mat TrackerCore::getFrame(){
	return this->frame;
}

bool TrackerCore::hasFrames(){
	this->num_frames--;

	if(num_frames <= 0)
		return false;

	return true;
}

void TrackerCore::saveDatabaseJson(){

}

void TrackerCore::generateGroundTruth(){
	uint num_frames = io::configs.video_frame_rate * io::configs.duration;
	Json::Value json;
 	Json::StyledWriter writer;
	json["frames"] =  Json::Value(Json::arrayValue);

	for(uint i = 0; i < num_frames; i++){
		Json::Value arr;
		json["frames"].append(arr);
		json["frames"][i]["frame"] = i;
		json["frames"][i]["num_trackers"] = this->num_trackers;
		json["frames"][i]["positions"] = Json::Value(Json::arrayValue);
		for(uint j = 0; j < this->num_trackers; j++){
			trackers[j].positions[i];
			Json::Value posic;
			json["frames"][i]["positions"].append(posic);
			json["frames"][i]["positions"][j]["frame_id"] = trackers[j].id;
			json["frames"][i]["positions"][j]["x_pos"] = trackers[j].positions[i].x;
			json["frames"][i]["positions"][j]["y_pos"] = trackers[j].positions[i].y;
		}
	}


	ofstream myfile;
	myfile.open (io::configs.ground_truth_file);
	myfile << writer.write(json);
	myfile.close();
}

uint Tracker::to_id = 0;

void Tracker::draw(){
	positions.push_back(Point(x_position, y_position));

	for(uint i = 0; i < image.cols; i++)
		for(uint j = 0; j < image.rows; j++)
			for(uint color = 0; color < 3; color++){
				if(image.at<Vec3b>(i, j)[color] != 0)
					frames.at<Vec3b>(
						this->y_position + i,
					 	this->x_position + j)[color] = image.at<Vec3b>(i, j)[color];
				}
}

void Tracker::generatePosition(){
	if(this->dead == true){
		this->recreate();
		return;
	}

	uint max_x = this->max_x_pos - (this->size_x + 2);
	uint max_y = this->max_y_pos - (this->size_y + 2);

	uint y_position, x_position;


	x_position = this->x_position;
	uint max = rand()%io::configs.max_mov + 1;
	uint moviment = rand()%max * pow(-1, rand()%2);
	x_position += moviment;
	if(!(x_position > size_x + 1 && x_position < max_x)){
		if(io::configs.no_bounded)
			this->dead = true;
		else
			x_position += -2*moviment;

	}
	this->x_position = x_position;



	y_position = this->y_position;
	max = rand()%io::configs.max_mov + 1;
	moviment = rand()%max * pow(-1, rand()%2);
	y_position += moviment;

	if(!(y_position > size_y + 1 && y_position < max_y)){
		if(io::configs.no_bounded)
			this->dead = true;
		else
			y_position += -2*moviment;
	}

	this->y_position = y_position;
}

void Tracker::recreate(){
	this->id = to_id++;

	double recreate = (rand()%100)/100;
	if(io::configs.recreate_chance < recreate){
		this->frames = frames;

		this->max_x_pos = frames.cols;
		this->max_y_pos = frames.rows;

		this->size_x = 10;
		this->size_y = 10;

		this->createColors();
		this->setPosition();
		this->createMat();

		this->dead = false;
	}
}
void Tracker::setPosition(){
	uint max_x = this->max_x_pos - 2*(this->size_x + 1);
	uint max_y = this->max_y_pos - 2*(this->size_y + 1);

	this->x_position = rand()%max_x + this->size_x + 1;
	this->y_position = rand()%max_y + this->size_x + 1;
}

Tracker::Tracker(Mat frames){
	this->id = to_id++;
	this->dead = false;

	this->frames = frames;

	this->max_x_pos = frames.cols;
	this->max_y_pos = frames.rows;

	this->size_x = io::configs.x_tracker_size;
	this->size_y = io::configs.y_tracker_size;

	this->setPosition();
	this->createColors();
	this->createMat();
}

void Tracker::createMat(){
	this->image = Mat::zeros(this->size_x, this->size_y, CV_8UC3);

	circle(this->image,
		Point(floor(this->size_x/2), floor(this->size_y/2)),
		floor(this->size_x/2) - 1,
		Scalar( red, blue, green),
		-1,
		8);

	for(uint i = 0; i < io::configs.num_features; i++){
		uint size_total = floor(this->size_x/2) - 1;
		uint size_parcial = ceil(this->size_x/4);
		uint max  = floor(size_total - size_parcial) - 1;
		//uint max = 1;
		int x_variation = (rand()%max) * pow(-1, rand()%2);
		int y_variation = (rand()%max) * pow(-1, rand()%2);

		Scalar color =  this->randomColor();

		circle(this->image,
			Point(floor(this->size_x/2) + x_variation, floor(this->size_y/2) + y_variation),
			ceil(this->size_x/4),
			color,
			-1,
			8);
	}

}

Scalar Tracker::randomColor(){
	return Scalar(rand()%256, rand()%256, rand()%256);
}

void Tracker::createColors(){
	this->red = rand()%128 + 128;
	this->blue = rand()%128 + 128;
	this->green = rand()%128 + 128;
}
