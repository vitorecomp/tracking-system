#include <vector>
#include <highgui.h>
#include <cv.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <string>

#include "Tracker.hpp"
#include "SurfTracker.hpp"
#include "Entities.hpp"

using namespace cv;
using namespace std;


int main(int argc, char *argv[])
{
	Timer timer;
	io::uni_args.decode(argc, argv);
	map<string, string> arguments = io::uni_args.getMap();

	//only change
	SurfTracker siftTrack;
	Tracker *tracker = (Tracker*)&siftTrack;


	tracker->reconfigure(arguments);

	Mat frame0 = Tracker::getFile(arguments["frame0"]);
	Mat frame1 = Tracker::getFile(arguments["frame1"]);


	int posx = stoi(arguments["center_x"], nullptr);
	int posy = stoi(arguments["center_y"], nullptr);

	int sizex = stoi(arguments["size_x"], nullptr);
	int sizey = stoi(arguments["size_y"], nullptr);

	Point position(posx, posy);
	Point size(sizex, sizey);

	Mat templ = Tracker::getTempl(frame1, position, size);


	timer.start();
	tracker->setTarget(templ, position, size);
	int set_time = timer.end();
	timer.reset();

	timer.start();
	tracker->track(frame0);
	uint track_time = timer.end();
	position = tracker->getPosition();

	waitKey(0);
	cout << "{" << endl;
	cout << "\t" << "\"setTime\": \"" << set_time << "\", "<< endl;
	cout << "\t" << "\"trackerTime\": \"" << track_time << "\"," << endl;
	cout << "\t" << "\"Position\":[" << position.x << "," << position.y << "]"<< endl;
	cout << "}" << endl;
	return 0;
}
