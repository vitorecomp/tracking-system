#include <iostream>
#include <stdlib.h>
#include <pthread.h>

//files
#include "./header/Tracker.hpp"

#include "./libs/output/IOMananger.hpp"

//defines
#define ESC 27

//namespaces
using namespace std;


//main
int main(int argc, char *argv[]) {
	stringstream msg;

    //io
    if(argc < 2){
        cout << "Por favor, entrar com os argumentos corretamente." << endl;
        cout << "./tracking_system configFile.json" << endl;
        cout << endl;
        cout << "configFile.json sendo um arquivo que contenha as configurações necessarias" << endl;
        cout << "para executar o programa" << endl;
        exit(1);
    }

    //gera as configuracoes
    io::configs.openFile(argv[1]);
    io::configs.setConfigs();
    thread io_th(run_io);

    //cria os parametros iniciais
    TrackerCore tracker_core;

    //cria populacoes
    tracker_core.startTrackers();

    //abrir videos
    VideoWriter output_video;

    //get parameters
    string video_name = tracker_core.getVideoName();
    uint fps  = tracker_core.getFrameRate();
    Size size = tracker_core.getVideoSize();
	static int codec = CV_FOURCC('M','J','P','G');

    //generate video
    output_video.open(video_name, codec, fps, size, true);
	if (!output_video.isOpened()){

		msg << "Could not open the output video for write: " << video_name << endl;
		io::output.printMsgBox("main_messages", msg.str());
		msg.str("");

  		io::input.endSignal();
	  	io_th.join();
      	return 0;
  	}

	uint i = 0;
    //sumariza informações
    while(tracker_core.hasFrames()){

        //gera as novas posiçoes
        tracker_core.generatePositions();

        //desenha
        tracker_core.draw();

        //pega o frame
        Mat frame = tracker_core.getFrame();

		msg << "Frame processo :" << i++;
		io::output.printMsgBox("main_messages", msg.str());
		msg.str("");
        //joga no videos
		//imshow("Teste", frame);
		//uint key = waitKey(10);
		//if(key == ESC)
		//	break;
        output_video << frame;
    }

	//generate json
	tracker_core.generateGroundTruth();

    //salva a info
    tracker_core.saveDatabaseJson();
	io::input.endSignal();
	io_th.join();
    return 0;
}
