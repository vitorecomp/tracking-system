//basics
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>

//opencv
#include <vector>
#include <highgui.h>
#include <cv.h>

//my
#include "../headers/Entities.hpp"
#include "../headers/Tracker.hpp"
#include "../headers/Brain.hpp"

using namespace std;
using namespace cv;

#define ESC 27

int main(int argc, char *argv[]){
	Arguments args;
    args.decode(argc, argv);
	io::uni_args = args;

	Text text(args.map_args["point_text_name"], args.map_args["start_text"]);

	//declara variaves
	VideoCapture cap(args.map_args["input_video"].c_str());

	if(!cap.isOpened()){  // check if we succeeded
		cout << "Erro ao abri o video" << endl;
		return -1;
	}
    //first Frame
    int fps = 2;
	Mat frame;
	cap >> frame;

	if(!frame.data)
		return -1;

    //initialize
    Timer timer;
    Brain brain(fps, frame, atoi(args.map_args["n_threads"].c_str()));

    for(uint i = 0; i < text.points.size(); i++)
	   brain.tracklets.push_back(Tracklet(frame, text.points[i], text.mask[i]));
	brain.buildOutput();
	imshow("frame", frame);
	imwrite("frame.jpg", frame);
	brain.init();
	//loop

	int c = 0;
	for(int i = 0;;i++){
		cap >> frame;
		if(!frame.data)
			break;
        //start timming
        timer.reset();
        timer.start();

        //brain
        brain.run(i);
        brain.reduce();

        //end timming
        timer.end();

        //brainDecisions
        brain.refactore(timer.getTotal());

        if(args.map_args["saving"].length() > 0)
            brain.buildOutput();

		imshow("frame", frame);
		waitKey(50);
		if(c == ESC)
			exit(0);

		if(args.map_args["saving_points"].length() > 0)
            for(int i =0; i < brain.numberObjects(); i++)
			    text.save(brain.objectPosic(i), i);

		if(args.map_args["saving_video"].length() > 0)
			brain.saveImage();
	}

	for(uint i = 0; i < brain.tracklets.size(); i++){
		brain.tracklets[i].saveGrapth();
	}

	brain.saveRelatorio();

	return 0;
}
