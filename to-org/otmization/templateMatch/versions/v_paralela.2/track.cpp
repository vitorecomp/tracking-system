#include "track.h"

Tracking::Tracking(double fps, Mat mainImage, Point center, Point size, double windowProp, bool debug, int n_threads, bool adaptable){

	Mat templ;
	templ = Mat(mainImage, Rect(center.x - size.x/2, center.y - size.y/2, size.x, size.y));

	this->fps = fps;
	this->debug = debug;
	this->templ = templ.clone();
	this->leftUp = Point(center.x - size.x/2, center.y - size.y/2);
	this->size = size;
	this->adaptable = adaptable;

	//calculo dos paramentros da janela
    //cout << windowProp << endl;
	this->winProp = windowProp;
    this->winSize = size * (windowProp - 1 + 0.5);
    this->shiftUp = this->winSize - size;

	//threads start
	this->n_threads = n_threads;

	this->id = 0;
	pthread_mutex_init(&this->m_id, NULL);

	this->work = 0;
	pthread_mutex_init(&this->m_work, NULL);
	pthread_cond_init(&this->s_work, NULL);

	this->finished = 0;
	pthread_mutex_init(&this->m_finished, NULL);
	pthread_cond_init(&this->s_finished, NULL);

	this->v_alive = true;
	pthread_mutex_init(&this->m_alive, NULL);

	//cout << "Cheguei" << endl;
	threads = (pthread_t*)calloc(n_threads, sizeof(pthread_t));
	minPontos = (Point*)calloc(n_threads, sizeof(Point));
	minValues = (double*)calloc(n_threads, sizeof(double));

	for(int i =0; i < n_threads; i++){
		int ret = pthread_create(&threads[i], NULL, thread_track, (void*)this);
		if (ret){
			cout << "Erro na criação de thread" << endl;
			exit(-1);
		}
	}
}

void Tracking::process(Mat frame)
{
	struct timeval start, end;
	gettimeofday(&start, NULL);
	Point minWindow = leftUp - shiftUp, idealMax = leftUp + winSize;
    Point realMax;
    //verifica se nao ta saindo da imagem
    this->realwin.x = (minWindow.x >= 0) ? minWindow.x : 0;
    this->realwin.y = (minWindow.y >= 0) ? minWindow.y : 0;

    realMax.x = (idealMax.x <= frame.cols) ? idealMax.x : frame.cols;
    realMax.y = (idealMax.y <= frame.rows) ? idealMax.y : frame.rows;

    this->realsize = realMax - realwin;
	this->frame = frame;

	launchWork();
	Point minLoc = findMin();

	rectangle(frame, minLoc + realwin, minLoc + realwin + size, Scalar( 0, 255, 255 ), 1, 8, 0);
    this->leftUp = minLoc + realwin;

    //verifica a adaptabilidade
    if(adaptable){
	    Mat ntempl(frame, Rect(minLoc + minWindow, Point(minLoc.x  + minWindow.x + size.x, minLoc.y + minWindow.y+ size.y)));
	    templ = ntempl.clone();
    }

    //verifica o debug
    if(debug){
        cout << "Posição real da janela: "<< realwin << endl;
        cout << "tamanho real da janela: "<< realsize << endl;
		imshow("template", templ);
//    	imshow("window", window);
        waitKey(20);
    	cout << minLoc << endl;
    }
}

int Tracking::getID(){
	int id = 0;
	pthread_mutex_lock(&m_id);
	id = this->id;
	this->id++;
	pthread_mutex_unlock(&m_id);
	return id;
}

bool Tracking::waitWork(int work){
	pthread_mutex_lock(&m_work);
		if(work >= this->work)
			pthread_cond_wait(&s_work, &m_work);
	pthread_mutex_unlock(&m_work);
	if(alive())
		return true;
	else
		return false;
}

bool Tracking::alive(){
	bool alive;
	pthread_mutex_lock(&m_alive);
	alive = v_alive;
	pthread_mutex_unlock(&m_alive);
	return alive;
}

void Tracking::launchWork(){
	//seta a finished = 0;
	pthread_mutex_lock(&m_finished);
	this->finished = 0;
	pthread_mutex_unlock(&m_finished);
	//lança sinal
	pthread_mutex_lock(&m_work);
	this->work++;
	pthread_mutex_unlock(&m_work);
	pthread_cond_broadcast(&s_work);
}


void* thread_track(void *tracking_void){
	Tracking *track = (Tracking*)tracking_void;
	int id = track->getID();
	int work = 0;
	ulint minValue = 0;
	Mat window;
	Point minPoint;
	bool alive;
	Mat result, local;
	double minVal; double maxVal; Point minLoc; Point maxLoc;

	while(track->alive()){
		alive = track->waitWork(work);
		if(!alive)
			break;
		try{
			//defiição da window
			window = track->makeWindow(id);

			//template match
		    int result_cols =  window.cols - track->templ.cols + 1;
		    int result_rows = window.rows - track->templ.rows + 1;
			result.create( result_rows, result_cols, CV_32FC1 );
		    matchTemplate(window, track->templ, result, CV_TM_SQDIFF);
			//min value
		    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
		    minPoint = minLoc;

			minPoint.x += (ceil((double)(track->realsize.x- track->size.x)/(double)track->n_threads) * id);
			track->setDone(id, minVal, minPoint);
		}catch (std::exception &e){
			cout << "Erro em " << id << endl;
			track->setDone(id, DBL_MAX, minPoint);
		}
		work++;
	}
	cout << "Thread " << id << "saiu" << endl;
}

void Tracking::setDone(int id, double minValue, Point ponto){
	this->minValues[id] = minValue;
	this->minPontos[id] = ponto;
	pthread_mutex_lock(&m_finished);
	this->finished++;
	pthread_cond_signal(&s_finished);
	pthread_mutex_unlock(&m_finished);
}

Mat Tracking::makeWindow(int id){
	int i = ceil((double)(realsize.x-size.x)/(double)n_threads);
	if(i * id > realsize.x)
		throw std::runtime_error("Gak!");
	int k = ((realwin.x + i * id + i + size.x) < frame.cols) ? i + size.x : (frame.cols - (realwin.x + i * (id)));
	Mat window(frame, Rect(realwin.x + i * id, realwin.y, k, realsize.y));
	return window;
}

Point Tracking::findMin(){
	while (1) {
		pthread_mutex_lock(&m_finished);
		if (this->finished  < this->n_threads)
			pthread_cond_wait(&s_finished, &m_finished);
		else
			break;
		pthread_mutex_unlock(&m_finished);
	}
	pthread_mutex_unlock(&m_finished);

	double minValue = DBL_MAX;
	Point minPoint;
	for(int i = 0; i < n_threads; i++){
		if(minValues[i] < minValue){
			minValue = minValues[i];
			minPoint = minPontos[i];
		}
	}
	return minPoint;
}
