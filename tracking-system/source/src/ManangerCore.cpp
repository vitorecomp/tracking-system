#include "../header/ManangerCore.hpp"

void ManangerCore::startTrackelets(){
    //pega os nomes dos frames das imagens
    this->getFrames(io::configs.video_name);

    //get start targets
    this->getTargetsPosition(io::configs.gt_name, 0);

    //get first image
    Mat first_frame = getFrame();

    //start threads
    makeTracklets(io::configs.threads);

	//define trckelet inicial
	defineStartTrackelet();

    //define metodo de distribuição
    defineDistMethod();

    //define a metrica de mudança
    defineMetric();
}

void ManangerCore::getFrames(std::string video_name){
	VideoCapture cap(video_name); // open the default camera
	if(!cap.isOpened()){
		//TODO Fazer o erro aqui
		return exit(1);
	}
	this->cap = cap;
}

Mat ManangerCore::getFrame(){
	Mat frame;
	cap >> frame;
	return frame;
}

void ManangerCore::getTargetsPosition(string name, uint frame){
	string line;
	stringstream ss;
	//open the text
	ifstream myfile;
	myfile.open(name, std::ifstream::in);

	//get the string
	if (myfile.is_open()){
		while(getline (myfile,line))
			ss << line << '\n';
		myfile.close();
	}else{
		throw std::invalid_argument("Json File invalid");
	}

	//validate the input
	string json = ss.str();
	if(json.length() == 0)
		throw std::invalid_argument("Json File invalid");


	//parse the string
	Json::Value value;
	Json::Reader reader;

	bool parsed = reader.parse(json, value);
	if(!parsed)
		throw std::invalid_argument("Json File invalid");

	//configure from json

	//

	const Json::Value values = value["frames"][frame]["positions"];
    for(uint i = 0; i < values.size(); ++i){
		Tracklet target;
		target.posic.x = values[i]["x_pos"].asInt();
		target.posic.y = values[i]["y_pos"].asInt();
		target.id = values[i]["frame_id"].asInt();
		this->tracklets.push_back(target);
    }
}


void ManangerCore::makeTracklets(uint threads){
	for(uint i = 0; i < threads; i++){
		blocks.push_back(TrackletBlock());
	}
}

void ManangerCore::run(){
    Mat frame;
    for(uint i = 0; i < frames.size(); i++){
        //open frame
        frame = openFrame(frames[i]);

        //set frame
        for(uint j = 0; j < tracklets.size(); j++){
            tracklets[j].setFrame(frame);
        }

        //get new tragets
        vector<Target> targets_inter = getNewPositons();

        //casos o metodo
        redistributeTargets(targets_inter);

        //execute tracklets
        executeAll();

        //avalia resultados
        evalResults();

        //make the decisions for next freme
        redefineMethods();
    }
}

void ManangerCore::buildOutputEnv(){
    //create the folders
    createFolders();

    //
}

void ManangerCore::generateInformation(){
    //make the graphs
    for(uint i = 0; i < tracklets.size(); i++)
        tracklets[i].saveGraph(this->outputs);

    //save the times
    for(uint i = 0; i < tracklets.size(); i++)
        tracklets[i].saveTimes(this->outputs);

    //sumirize imformation
    sumirizeTimeReport();
    sumirizeErrorReport();
    sumirizeChangeReport();
    sumirizeFullReport();
}

void ManangerCore::startUniversalQueue(){
	this->universal_queue.start();
}

ManangerCore::DIST_METHOD ManangerCore::convertDistMethod(string method){
	if(method.compare("STATIC") == 0){
		return DIST_METHOD::STATIC;
	}

	if(method.compare("DYNAMIC") == 0){
		return DIST_METHOD::DYNAMIC;
	}
	return DIST_METHOD::STATIC;

}

void ManangerCore::defineDistMethod(){
    this->dist_method = convertDistMethod(io::configs.dist_method);
    switch (this->dist_method) {
        case STATIC:
            //se estatico
            for(uint i = 0; i < blocks.size(); i++)
                blocks[i%io::configs.threads].addTarget(tracklets[i]);

            break;
        case DYNAMIC:
            //se dinamico
            startUniversalQueue();
            for(uint i = 0; i < tracklets.size(); i++)
                blocks[i].addQueue(&universal_queue);
            break;

        default:
            io::input.endSignal();
            throw std::invalid_argument("Escalonador invalido");
    }
}


ManangerCore::METHOD ManangerCore::convertMethod(string method){
	if(method.compare("TEMPLATE_MACTHING") == 0){
		return METHOD::TEMPLATE_MACTHING;
	}

	if(method.compare("SURF") == 0){
		return METHOD::SURF;
	}

	if(method.compare("SIFT") == 0){
		return METHOD::SIFT;
	}

	if(method.compare("BACKGROUND_SUBTRACTION") == 0){
		return METHOD::BACKGROUND_SUBTRACTION;
	}

	return METHOD::TEMPLATE_MACTHING;
}

void ManangerCore::defineStartTrackelet(){
    this->method = convertMethod(io::configs.inicial_method);
    switch (this->method) {
        case TEMPLATE_MACTHING:
            //template matching
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMethod(Tracklet::TEMPLATE_MACTHING);
            break;

        case SURF:
            //surf
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMethod(Tracklet::SURF);
            break;

        case SIFT:
            //sift
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMethod(Tracklet::SIFT);
            break;

        case BACKGROUND_SUBTRACTION:
            //backgrund substrations
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMethod(Tracklet::BACKGROUND_SUBTRACTION);
            break;

        default:
            io::input.endSignal();
            throw std::invalid_argument("Meto invalido");

    }
}

ManangerCore::METRIC ManangerCore::convertMetric(string metric){
	if(metric.compare("TIME") == 0){
		return METRIC::TIME;
	}

	if(metric.compare("ACURACY") == 0){
		return METRIC::ACURACY;
	}
	return METRIC::TIME;
}

void ManangerCore::defineMetric(){
    this->metric = convertMetric(io::configs.metric);
    switch (this->metric) {
        case TIME:
            //se time
            for(uint i = 0; i < tracklets.size(); i++)
                blocks[i].setMetric(TrackletBlock::TIME);
            break;
        case ACURACY:
            //se acuraci
            for(uint i = 0; i < tracklets.size(); i++)
                blocks[i].setMetric(TrackletBlock::ACURACY);
            break;

        default:
            io::input.endSignal();
            throw std::invalid_argument("Metrica invalida");
    }
}
