#ifndef TRACK_H
#define TRACK_H

#include <highgui.h>
#include <cv.h>
#include <sys/time.h>
#include <iostream>
#include <pthread.h>
#define ullint unsigned long long int


#define NUM_THREADS    400
#define ulint unsigned long int

using namespace cv;
using namespace std;

class Tracking
{
private:
	double fps;
	//kalman positon
	Point kalman;
	//Alrith Position
	Point leftUp;
	Point winSize;
	Point shiftUp;



	//debug
	bool debug;
	bool adaptable;
	double winProp;


	//parallel
	int id;
	pthread_mutex_t m_id;

	int work;
	pthread_mutex_t m_work;
	pthread_cond_t s_work;

	int finished;
	pthread_mutex_t m_finished;
	pthread_cond_t s_finished;


	bool v_alive;
	pthread_mutex_t m_alive;


	//kalman variables



public:
	Point size;
	Point realsize, realwin;
	int n_threads;

	pthread_t *threads;
	Point *minPontos;
	ulint *minValues;
	Mat *windows;
	Tracking(double, Mat mainImage, Point, Point, double , bool, int, bool);
	void process(Mat nextImage);
	Point mymatchTemplate(Mat , Mat, ulint*);

	//thread funcs
	Mat templ;
	Mat frame;
	int getID();
	void launchWork();
	Point findMin();
	bool alive();
	bool waitWork(int);
	Mat makeWindow(int id);
	void setDone(int, ulint, Point);

};
inline ullint time_to_int(struct timeval start, struct timeval end){
    return 	((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
};
	void* thread_track(void *tracking_void);

#endif
