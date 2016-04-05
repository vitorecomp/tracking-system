#ifndef BASICS_H
#define BASICS_H

#include <cstdlib>
#include <sys/time.h>
#include <pthread.h>


typedef struct timeval time_us;
typedef unsigned long long int ulint;

#define ESC 27

class Time{
private:
    struct timeval start_tim, end_tim;
    uint total;
public:
    static uint time_to_int(struct timeval, struct timeval);
    Time();
    void reset();
    void start();
    void end();
    uint getTotal();
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


#endif
