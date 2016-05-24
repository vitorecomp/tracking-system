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
	minValues = (uint*)calloc(n_threads, sizeof(uint));

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
	unsigned int minValue = 0;
	Point minPoint;
	bool alive;

	while(track->alive()){
		alive = track->waitWork(work);
		if(!alive)
			break;
		minPoint = track->mymatchTemplate(id, &minValue);
		track->setDone(id, minValue, minPoint);
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
	pthread_cond_signal(&s_finished);
	pthread_mutex_unlock(&m_finished);
}


void Tracking::process(Mat frame)
{
	Point minWindow = leftUp - shiftUp, idealMax = leftUp + winSize;
    Point realMax;
    //verifica se nao ta saindo da imagem
    this->realwin.x = (minWindow.x >= 0) ? minWindow.x : 0;
    this->realwin.y = (minWindow.y >= 0) ? minWindow.y : 0;

    realMax.x = (idealMax.x <= frame.cols) ? idealMax.x : frame.cols;
    realMax.y = (idealMax.y <= frame.rows) ? idealMax.y : frame.rows;

    this->realsize = realMax - realwin;

	this->frame = frame;
	//work
	launchWork();
	//find min betwen threads
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
        waitKey(20);
    		cout << minLoc << endl;
    }
}

Point Tracking::mymatchTemplate(int id, unsigned int *minValue){

	    uchar *lineT, *lineW, *end, *endF;

		unsigned int mindistance = 200000000;
	    unsigned int distance;

	    int diff = frame.step - templ.step;
		unsigned int p;
		int dx = 0, dy = 0, mx = 0, my = 0;


	    endF = templ.data + templ.step * templ.rows;
	    uchar *dataW = frame.data + (realwin.y*frame.step) + (realwin.x*3);

	    int puloX = realsize.x - templ.cols;
	    int puloY = realsize.y - templ.rows;
	    int maxPulos = puloX * puloY;
		int n_threads = this->n_threads;

		for(int i = id; i < maxPulos; i+=n_threads){
	            dx = i % puloX;
	            dy = i / puloX;
	            //inicializaçao
				distance = 0;
	            lineW = dataW + dx*3 + dy * frame.step;
	            end = templ.data;
	            lineT = end;

	            //diff
				while(lineT != endF){
	                end += templ.step;
					for(;lineT != end;lineT++, lineW++){
	                    p = *(lineT++) - *(lineW++);
						distance +=  p * p;
	                    p = *(lineT++) - *(lineW++);
						distance += p * p;
	                    p = *(lineT) - *(lineW);
						distance += p * p;
					}
	                lineW += diff;
	                //cout << distance << " em i = " << i++ << endl;
	                //getchar();
				}

	            //min dcision
				if(distance <= mindistance){
					mindistance = distance;
	                mx = dx;
	                my = dy;
	            }

			}

		//cout << minPoint << endl;
		//cout << mindistance << endl;
		*minValue = mindistance;
		return Point(mx, my);
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
	int i = ceil((double)realsize.x/(double)n_threads);
	//cout << "Meu valor de size"<< realsize.x << endl;
	int k = ((realwin.x + i * id + i + size.x) < frame.cols) ? i + size.x : (frame.cols - (realwin.x + i * id));
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
	gettimeofday(&start, NULL);
	while (1) {
		pthread_mutex_lock(&m_finished);
		if (this->finished  < this->n_threads)
			pthread_cond_wait(&s_finished, &m_finished);
		else
			break;
		pthread_mutex_unlock(&m_finished);
	}
	pthread_mutex_unlock(&m_finished);
	gettimeofday(&end, NULL);
	//cout << "time to sync " <<time_to_int(start, end) << endl;


	gettimeofday(&start, NULL);
	int minValue = 1000000000;
	Point minPoint;
	for(int i = 0; i < n_threads; i++){
		//cout << "Ponto mininmo " << minPontos[i] << endl;
		//imshow("window", windows[i]);
		//waitKey(0);
		if(minValues[i] < minValue){
			minValue = minValues[i];
			minPoint = minPontos[i];
		}
	}
	gettimeofday(&end, NULL);
	//cout << "find min " <<time_to_int(start, end) << endl;
	return minPoint;
}
