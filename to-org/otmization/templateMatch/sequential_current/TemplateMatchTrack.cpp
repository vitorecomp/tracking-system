#include "./TemplateMatchTrack.hpp"

Point TemplateMatchTracker::getPosition(){
	return actual_position;
}

void TemplateMatchTracker::track(Mat frame){
	Point minWindow = leftUp - shiftUp, idealMax = leftUp + winSize;
    Point realMax;
    //verifica se nao ta saindo da imagem
    realwin.x = (minWindow.x >= 0) ? minWindow.x : 0;
    realwin.y = (minWindow.y >= 0) ? minWindow.y : 0;

    realMax.x = (idealMax.x <= frame.cols) ? idealMax.x : frame.cols;
    realMax.y = (idealMax.y <= frame.rows) ? idealMax.y : frame.rows;

    realsize = realMax - realwin;
    this->frame = frame;


    //template matching
	//Mat window(frame, Rect(realwin.x, realwin.y, realsize.x, realsize.y));
    Point minLoc  = mymatchTemplate();
    rectangle(frame, minLoc + realwin, minLoc + realwin + size, Scalar( 0, 255, 255 ), 1, 8, 0);
    this->leftUp = minLoc + realwin;
	this->actual_position = Point(leftUp.x + size.x/2, leftUp.y + size.y/2);

    //verifica a adaptabilidade
    if(adaptable == true){
	    Mat ntempl(frame, Rect(minLoc + minWindow, Point(minLoc.x  + minWindow.x + size.x, minLoc.y + minWindow.y+ size.y)));
	    templ = ntempl.clone();
    }

    //verifica o debug
    //if(debug){
        //cout << "Posição real da janela: "<< realwin << endl;
        //cout << "tamanho real da janela: "<< realsize << endl;
    	//imshow("window", window);
		//imshow("template", templ);
		//waitKey(40);
    	//	cout << minLoc << endl;
    //}
}

void TemplateMatchTracker::setTarget(Mat templ, Point center, Point size){
	int height = templ.rows;
    int width = templ.cols;
	Point positionUp, totalX;

	this->templ = templ.clone();
	this->leftUp = Point(center.x - size.x/2, center.y - size.y/2);
	this->size = size;

	//calculo dos paramentros da janela
    this->winSize = size * (windowProp - 1 + 0.5);
    this->shiftUp = this->winSize - size;
}

void TemplateMatchTracker::reconfigure(map<string, string> args){
	std::map<string,string>::iterator it;
	it = args.find("adaptable");
  	if (it != args.end())
		this->adaptable = args["adaptable"].compare("True") == 0;
	else
		this->adaptable = false;


	it = args.find("win_prop");
  	if (it != args.end())
		windowProp = stoi(args["win_prop"]);
	else
		windowProp = 2;
}

Point TemplateMatchTracker::mymatchTemplate(){

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

	for(int i = 0; i < maxPulos; i++){
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

	return Point(mx, my);
}
