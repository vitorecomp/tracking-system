#ifndef BASICS_H
#define BASICS_H

#include <cstdlib>
#include <sys/time.h>
#include <pthread.h>


#ifndef ullint
    #define ullint unsigned long long int
#endif

#define ESC 27

class Time{
private:
    struct timeval start_tim, end_tim;
    ullint total;
    ullint time_to_int(struct timeval, struct timeval);
public:
    Time();
    void reset();
    void start();
    void end();
    ullint getTotal();
};

class Counter{
    int max;
    int finished;
    pthread_mutex_t m_finished;
    pthread_cond_t s_finished;
public:
    Counter();
    Counter(int);

    void waitMax();
    void increment();
};

class Blocker{
    pthread_mutex_t m_work;
	pthread_cond_t s_work;
    int last;
public:
    Blocker();
    void run();
    int waitRunSignal(int last);
};


inline ullint Time::time_to_int(struct timeval start, struct timeval end){
    return 	((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
}
#endif
