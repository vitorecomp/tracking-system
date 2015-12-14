#include "../header/ManangerCore.hpp"

void ManangerCore::startTrackelets(){
    //pega os nomes dos frames das imagens
    getFrames(io::configs.videofolder)

    //get start targets
    makeTargets(io::configs.gt_name, 0);

    //get first image
    Mat first_frame = openFrame(frames[0]);

    //start threads
    makeTracklets(io::configs.threads);

    //define metodo de distribuição
    defineDistMethod();

    //define trckelet inicial
    defineStartTrackelet();

    //define a metrica de mudança
    defineMetric();
}

void ManangerCore::run(){
    Mat frame;
    for(uint i = 0; i < frames.size(); i++){
        //open frame
        frame = openFrame(frame[i]);

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

void ManangerCore::defineDistMethod(){
    this->dist_method = convertMethod(io::configs.dist_method);
    switch (this->dist_method) {
        case STATIC:
            //se estatico
            for(uint i = 0; i < targets.size(); i++)
                tracklets[i%io::configs.threads].setTarget(tragets[i]);

            break;
        case DYNAMIC:
            //se dinamico
            startUniversalQueue();
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setTarget(&universal_queue);
            break;

        default:
            io::input.endSignal();
            throw std::invalid_argument("Escalonador invalido");
    }
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

void ManangerCore::defineMetric(){
    this->metric = convertMetric(io::configs.metric);
    switch (this->metric) {
        case TIME:
            //se time
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMetric(Tracklet::TIME);
            break;
        case ACURACY:
            //se acuraci
            for(uint i = 0; i < tracklets.size(); i++)
                tracklets[i].setMetric(Tracklet::ACURACY);
            break;

        default:
            io::input.endSignal();
            throw std::invalid_argument("Metrica invalida");
    }
}
