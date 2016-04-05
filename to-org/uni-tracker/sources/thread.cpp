#include "../headers/Thread.hpp"

void* run_thread(void *arg){
	Thread *thread = (Thread*)arg;
	thread->waitSignal();
	while(1) {
		thread->run();
	}
	return NULL;
}

Thread::Thread(){
	running = false;
	pthread_mutex_init(&this->runMutex_m, NULL);
	pthread_cond_init(&this->runSignal_s, NULL);
	thread::threadInit(runThread_t, NULL, run_thread, (void*)this);

}

void Thread::runSignal(){
	pthread_mutex_lock(&runMutex_m);
	running = true;
	pthread_cond_signal(&runSignal_s);
	pthread_mutex_unlock(&runMutex_m);
}

void Thread::waitSignal(){
	pthread_mutex_lock(&runMutex_m);
	if(!running)
			pthread_cond_wait(&runSignal_s, &runMutex_m);
	pthread_mutex_unlock(&runMutex_m);
}
