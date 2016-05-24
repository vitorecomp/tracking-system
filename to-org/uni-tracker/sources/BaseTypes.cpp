#include "../headers/BaseTypes.hpp"


Time::Time(){
	gettimeofday(&start_tim, NULL);
	gettimeofday(&end_tim, NULL);
	total = 0;
}
void Time::reset(){
	total = 0;
}
void Time::start(){
	gettimeofday(&start_tim, NULL);
}
void Time::end(){
	gettimeofday(&end_tim, NULL);
	total = time_to_int(start_tim, end_tim);
}
uint Time::getTotal(){
	return total;
}


Blocker::Blocker(){
	last = 0;
	pthread_mutex_init(&this->m_work, NULL);
	pthread_cond_init(&this->s_work, NULL);
}

void Blocker::run(){
	pthread_mutex_lock(&m_work);
	this->last++;
	pthread_mutex_unlock(&m_work);
	pthread_cond_broadcast(&s_work);
}

int Blocker::waitRunSignal(int last){
	pthread_mutex_lock(&m_work);
		if(last >= this->last)
			pthread_cond_wait(&s_work, &m_work);
	pthread_mutex_unlock(&m_work);
	last++;
	return last;
}

Counter::Counter(){
	this->finished = 0;
	pthread_mutex_init(&this->m_finished, NULL);
	pthread_cond_init(&this->s_finished, NULL);
	max = 1;
}

Counter::Counter(int max){
	this->finished = 0;
	pthread_mutex_init(&this->m_finished, NULL);
	pthread_cond_init(&this->s_finished, NULL);
	this->max = max;
}

void  Counter::waitMax() {
	while (1) {
		pthread_mutex_lock(&m_finished);
		if (this->finished  < this->max)
			pthread_cond_wait(&s_finished, &m_finished);
		else
			break;
		pthread_mutex_unlock(&m_finished);
	}
	finished = 0;
	pthread_mutex_unlock(&m_finished);
}

void Counter::increment(){
	pthread_mutex_lock(&m_finished);
	this->finished++;
	pthread_cond_signal(&s_finished);
	pthread_mutex_unlock(&m_finished);
}


uint Time::time_to_int(struct timeval start, struct timeval end){
    return 	((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
}
