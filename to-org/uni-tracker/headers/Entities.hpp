#ifndef ENTITIES_H
#define ENTITIES_H

#include <highgui.h>
#include <cv.h>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <sys/time.h>
#include <iostream>

#include "../libs/tinyxml.h"
#include "./BaseTypes.hpp"

using namespace std;
using namespace cv;
#define ullint unsigned long long int


class Timer{
	ullint time_to_int(struct timeval start, struct timeval end);
	//variaveis temporais
    ullint total;
    struct timeval start_v, end_v;
public:
	Timer();
	void start();
	void reset();
	ullint end();

	ullint getTotal();
};


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

class Arguments{
public:
	map<string, string> map_args;
	map<string, string> getMap();
	void decode(int, char**);
};

namespace io{
	extern Arguments uni_args;
};

inline ullint Timer::time_to_int(struct timeval start, struct timeval end){
    return 	((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
};

#endif
