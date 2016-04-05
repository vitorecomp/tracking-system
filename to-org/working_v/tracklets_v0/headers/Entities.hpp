#ifndef ENTITIES_H
#define ENTITIES_H

#include "Thread.hpp"
#include "BaseTypes.hpp"
#include "../libs/tinyxml.h"

#include <iostream>
#include <string>
#include <vector>

//opencv
#include <vector>
#include <highgui.h>
#include <cv.h>

using namespace cv;
using namespace std;


class Text{
	string result;
	string input;

	string element;
	int nFrame;
	void dump_attribs_to_stdout(TiXmlElement* pElement);
	void dump_to_stdout( TiXmlNode* pParent);
public:
	Text(string, string);

	void save(Point, int);

	vector<Point> points;
	vector<Point> mask;
};

class Argument{
	//resultado
	string pointTextName;

	//entrada
	string startText;
	string videoName;


public:
	Argument(int, char**);
	Argument();

	string getPointTextName();
	string getStartText();
	string getVideoName();

	int nthreads;
	bool saving;
	bool debug;
	bool savingPoints;
	bool savingVideo;
	bool saveImage;
};

class Output : public Thread{
private:
    pthread_mutex_t active_m;
    bool active;
public:
    Output();
    void activate();
    void run();
};

class Debug : public Thread{
private:
    pthread_mutex_t active_m;
    bool active;
public:
    Debug();
    void activate();
    void run();
};


namespace io{
	extern Argument uniArgs;
    extern Output output;
    extern Debug debug;
}

#endif
