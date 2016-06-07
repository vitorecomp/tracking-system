//includes
#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <cmath>

#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "./libs/jsonParser/json.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[]){
	if(argc < 3){
		cout << "Error, enter with alov++ database video path";
		cout << "and the result path off the video";
		return 1;
	}

	string path = argv[1];
	string result_path = argv[2];
	vector<string> frames;


	DIR *pdir = NULL;
	cout << path << endl;
    pdir = opendir (path.c_str());
    struct dirent *pent = NULL;

    while (pent = readdir(pdir)){
        if(pent == NULL)
            continue;

        if(!string(pent->d_name).compare("."))
            continue;
        if(!string(pent->d_name).compare(".."))
            continue;

		if(string(pent->d_name).at(0) == '.')
			continue;

		string valor = path + "/" + string(pent->d_name);
		frames.push_back(valor);
    }

    closedir (pdir);


	Mat frame;
    frame = imread(frames[0], CV_LOAD_IMAGE_COLOR);

	VideoWriter output_video;

	uint fps  = 10;
    Size size(frame.cols, frame.rows);
	static int codec = CV_FOURCC('M','J','P','G');

    //generate video
    output_video.open(result_path, codec, fps, size, true);
	if (!output_video.isOpened()){
		cout << "Could not open the output video for write: " << result_path << endl;
		return -1;
  	}

    for(uint i = 0; i < frames.size(); i++){
		frame = imread(frames[i], CV_LOAD_IMAGE_COLOR);
        output_video << frame;
    }


    return 0;
}
