//basics
#include <iostream>
#include <cstdlib>
#include <cmath>

//opencv
#include <vector>
#include <highgui.h>
#include <cv.h>

//my
#include "../headers/Tracking.hpp"
#include "../headers/BaseTypes.hpp"
#include "../headers/Brain.hpp"
#include "../headers/Entities.hpp"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	Argument args(argc, argv);
	io::uniArgs = args;

	Text text(args.getPointTextName(), args.getStartText());

	//declara variaves
	VideoCapture cap(args.getVideoName().c_str());

	if(!cap.isOpened()){  // check if we succeeded
		cout << "Erro ao abri o video" << endl;
		return -1;
	}
    //first Frame
    int fps = 5;
	Mat frame;
	cap >> frame;

	if(!frame.data)
		return -1;

    //initialize
    Time timer;
    Brain brain(fps, frame, args.nthreads);
    for(uint i = 0; i < text.points.size(); i++)
	   brain.tracklets.push_back(Tracklet(frame, text.points[i], text.mask[i]));
	brain.buildOutput();
	imshow("frame", frame);
	//waitKey(0);
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

        if(args.saving)
            brain.buildOutput();

        if(args.debug)

		imshow("frame", frame);
		//imwrite("frame2.jpg", frame);
		//c = waitKey(0);
		if(c == ESC)
			exit(0);

		if(args.savingPoints)
            for(int i =0; i < brain.numberObjects(); i++)
			    text.save(brain.objectPosic(i), i);

		if(args.savingVideo)
			brain.saveImage();
	}

	return 0;
}
