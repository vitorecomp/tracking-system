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

    match_method = CV_TM_SQDIFF;
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
    Mat window(frame, Rect(realwin.x, realwin.y, realsize.x, realsize.y));

    /// Create the result matrix
    int result_cols =  window.cols - templ.cols + 1;
    int result_rows = window.rows - templ.rows + 1;

    result.create( result_rows, result_cols, CV_32FC1 );

    /// Do the Matching and Normalize
    matchTemplate(window, templ, result, match_method);
    normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );

    /// Localizing the best match with minMaxLoc
    double minVal; double maxVal; Point minLoc; Point maxLoc;
    Point matchLoc;

    minMaxLoc( result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );

    /// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
    if( match_method  == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED )
        matchLoc = minLoc;
    else
        matchLoc = maxLoc;

    rectangle(frame, matchLoc + realwin, minLoc + realwin + size, Scalar( 0, 255, 255 ), 1, 8, 0);


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

	unsigned long int mindistance = 20000000000;

	double minpointDistance = 10000000000;
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
			unsigned long int distance = 0;
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
	//cout << mindistance << endl;

	return minPoint;
}
