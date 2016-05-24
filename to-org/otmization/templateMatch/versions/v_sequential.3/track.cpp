#include "track.h"

Tracking::Tracking(double fps, Mat mainImage, Point center, Point size, double windowProp, bool debug, bool adaptable){


    int height = mascara.rows;
    int width = mascara.cols;
	Point positionUp, totalX;

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
}


void Tracking::process(Mat frame)
{
    Point minWindow = leftUp - shiftUp, idealMax = leftUp + winSize;
    Point realwin, realsize, realMax;
    //verifica se nao ta saindo da imagem
    realwin.x = (minWindow.x >= 0) ? minWindow.x : 0;
    realwin.y = (minWindow.y >= 0) ? minWindow.y : 0;

    realMax.x = (idealMax.x <= frame.cols) ? idealMax.x : frame.cols;
    realMax.y = (idealMax.y <= frame.rows) ? idealMax.y : frame.rows;

    realsize = realMax - realwin;


    //template matching
	Mat window(frame, Rect(realwin.x, realwin.y, realsize.x, realsize.y));
    Point minLoc  = mymatchTemplate(window, templ);
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
    	imshow("window", window);
    	imshow("template", templ);
        waitKey(40);
    		cout << minLoc << endl;
    }
}

Point Tracking::mymatchTemplate(Mat window, Mat templ){

    uchar *lineT, *lineW, *end, *endF;

	unsigned int mindistance = 200000000;
    unsigned int distance;
    int diff = window.step - templ.step;
	unsigned int p;
	int dx = 0, dy = 0, mx = 0, my = 0;

    endF = templ.data + templ.step * templ.rows;
	for(dx = 0; dx < window.cols - templ.cols; dx++)
		for(dy = 0; dy < window.rows - templ.rows; dy++){
            //inicializaçao
			distance = 0;
            lineW = window.data + dx*3 + dy * window.step;
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

	return Point(mx, my);
}
