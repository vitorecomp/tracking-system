#ifndef ENTITIES_H
#define ENTITIES_H

#include <cstdlib>
#include <cstdio>
#include <map>
#include <sys/time.h>
#include <iostream>

using namespace std;
#define ullint unsigned long long int


class Timer{
	ullint time_to_int(struct timeval start, struct timeval end);
	//variaveis temporais
    ullint total = 0;
    struct timeval start_v, end_v;
public:
	Timer();
	void start();
	void reset();
	ullint end();
};

class Arguments{
	map<string, string> map_args;
public:
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
