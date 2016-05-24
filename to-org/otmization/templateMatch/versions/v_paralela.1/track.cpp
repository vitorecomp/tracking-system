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
	//cout << "Cheguei" << endl;
	minPontos = (Point*)calloc(n_threads, sizeof(Point));
	minValues = (ulint*)calloc(n_threads, sizeof(ulint));
	windows = (Mat*)calloc(n_threads, sizeof(Mat));

	for(int i =0; i < n_threads; i++){
		int ret = pthread_create(&threads[i], NULL, thread_track, (void*)this);
		if (ret){
			cout << "Erro na criação de thread" << endl;
			exit(-1);
		}
	}
}

void* thread_track(void *tracking_void){
	Tracking *track = (Tracking*)tracking_void;
	int id = track->getID();
	int work = 0;
	ulint minValue = 0;
	Mat window;
	Point minPoint;
	bool alive;

	while(track->alive()){
		alive = track->waitWork(work);
		if(!alive)
			break;
		try{
			window = track->makeWindow(id);
			minPoint = track->mymatchTemplate(window, track->templ, &minValue);
			minPoint.x += ((double)(track->realsize.x - track->size.x)/(double)track->n_threads) * id;
			//track->windows[id] = window;
			//cout << "Thread " << id << " chegou aqui" << endl;
			track->setDone(id, minValue, minPoint);
		}catch (std::exception &e){
			//cout << "Erro em " << id << endl;
			track->setDone(id, 429496729, minPoint);
		}
		work++;
	}
	cout << "Thread " << id << "saiu" << endl;
}


void Tracking::setDone(int id, ulint minValue, Point ponto){
	//cout << "Thread " << id << " Min value " << minValue << " " << ponto << endl;
	this->minValues[id] = minValue;
	this->minPontos[id] = ponto;
	pthread_mutex_lock(&m_finished);
	this->finished++;
	if(debug)
		cout << finished << endl;
	pthread_cond_signal(&s_finished);
	pthread_mutex_unlock(&m_finished);
}


void Tracking::process(Mat frame)
{
	//struct timeval start, end;
	//gettimeofday(&start, NULL);
	Point minWindow = leftUp - shiftUp, idealMax = leftUp + winSize;
    Point realMax;
    //verifica se nao ta saindo da imagem
    this->realwin.x = (minWindow.x >= 0) ? minWindow.x : 0;
    this->realwin.y = (minWindow.y >= 0) ? minWindow.y : 0;

    realMax.x = (idealMax.x <= frame.cols) ? idealMax.x : frame.cols;
    realMax.y = (idealMax.y <= frame.rows) ? idealMax.y : frame.rows;

    this->realsize = realMax - realwin;


    //template matching
	//Mat window(frame, Rect(realwin.x, realwin.y, realsize.x, realsize.y));
	this->frame = frame;
	//lança trabalho
	//cout << "vou Lancar "<< realwin << endl;
	//gettimeofday(&end, NULL);
	//cout << time_to_int(start, end) << endl;

	//gettimeofday(&start, NULL);
	launchWork();
	//find min betwen threads
	//cout << "Lancei" << endl;
	Point minLoc = findMin();
	//gettimeofday(&end, NULL);
	//cout << "sync " <<time_to_int(start, end) << endl;
	//cout << "Cheguei aqui" << endl;

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

Point Tracking::mymatchTemplate(Mat window, Mat templ, ulint *minValue){

	ulint mindistance = 200000000;
	ulint distance = 0;

	double minpointDistance = 10000000;
	Point center, minPoint;

	center.x = window.rows/2;
	center.y = window.cols/2;

	uchar *templdata = templ.data;
	uchar *windowdata = window.data;

	int p, p1, p2;
	int x1, x2, x3;
	int y1, y2, y3;
	for(int dx = 0; dx < window.cols - templ.cols; dx++){
		for(int dy = 0; dy < window.rows - templ.rows ; dy++){
			distance = 0;
			y3  = dy*window.step;
			x3 = dx*3;
			//Mat a(window, Rect(dx, dy, size.x, size.y));
			//cout << a.size << endl;
			//cout << templ.size << endl;
			//distance = norm(a, templ, NORM_L2);


			for(int y = 0; y < templ.rows; y++){
				for(int x = 0; x < templ.cols; x++){

					x1 = x*3;
					x2 = x1 + x3;

					y1 = y*templ.step;
					y2 = y*window.step + y3;

					p =  windowdata[x2 + y2] - templdata[x1 + y1] ;
					distance +=  p * p;
					p1 =  windowdata[x2 + y2 + 1] - templdata[x1 + y1 + 1];
					distance += p1 * p1;
					p2 = windowdata[x2 + y2 + 2] - templdata[x1 + y1 + 2] ;
					distance += p2 * p2;

				}
			}




			if(distance <= mindistance){
				if(distance ==  mindistance){
					double resL = norm(Point(dx, dy)-center);
					if(resL <= minpointDistance)
					{
						minPoint.x = dx;
						minPoint.y = dy;
						minpointDistance = resL;
					}
				}
				minPoint.x = dx;
				minPoint.y = dy;
				mindistance = distance;
			}
		}
	}
	//cout << minPoint << endl;
	*minValue = mindistance;
	return minPoint;
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

Mat Tracking::makeWindow(int id){
	int i = ceil((double)(realsize.x-size.x)/(double)n_threads);
	//cout << "Meu valor de size"<< realsize.x << endl;
	if(i * id > realsize.x)
		throw std::runtime_error("Gak!");
	int k = ((realwin.x + i * id + i + size.x) < frame.cols) ? i + size.x : (frame.cols - (realwin.x + i * (id)));
	//cout << "Thread " << id << "saiu com id " << realwin.x + i * id << endl;
	Mat window(frame, Rect(realwin.x + i * id, realwin.y, k, realsize.y));
	return window;
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


Point Tracking::findMin(){
	struct timeval start, end;
	//gettimeofday(&start, NULL);
	while (1) {
		pthread_mutex_lock(&m_finished);
		if (this->finished  < this->n_threads)
			pthread_cond_wait(&s_finished, &m_finished);
		else
			break;
		pthread_mutex_unlock(&m_finished);
	}
	pthread_mutex_unlock(&m_finished);
	//gettimeofday(&end, NULL);
	//cout << "time to sync " <<time_to_int(start, end) << endl;


	// /gettimeofday(&start, NULL);
	ulint minValue = 1000000000;
	Point minPoint;
	for(int i = 0; i < n_threads; i++){
		//cout << "Ponto mininmo " << minPontos[i] << endl;
		if(minValues[i] < minValue){
			minValue = minValues[i];
			minPoint = minPontos[i];
		}
	}
	//imshow("window", frame);
	//waitKey(0);
	//gettimeofday(&end, NULL);
	//cout << "find min " <<time_to_int(start, end) << endl;
	return minPoint;
}
