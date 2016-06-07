#include <iostream>
#include <stdlib.h>
#include <pthread.h>

//files

#include <highgui.h>
#include <cv.h>
#include <iostream>
//defines

//namespaces
using namespace std;
using namespace cv;

#define ALT 600
#define LAR 10
#define MAXL 50*16

//main
int main(int argc, char *argv[]) {
	int vec[4][16] = {
		{40, 20, 18, 18, 	17, 17, 14, 14, 10, 	9, 9, 9, 8, 8, 9, 8},
		{0, 20, 14, 16, 16, 13, 12, 8, 9, 13, 		13, 13, 11, 13, 13, 14},
		{0, 0, 6, 5, 5, 	9, 	9, 	12, 11, 10, 	10, 11, 13, 11, 10, 10},
		{0, 0, 0, 1, 1, 	1,	2, 	1, 	1, 	1, 		1, 	0, 	0, 	0, 1, 1}
	};


	Mat image = Mat::ones(ALT, MAXL, CV_8UC3);
	for(uint i = 0; i < image.cols*image.rows*image.channels(); i++){
			image.data[i] = 255;
	}
	cout << "aqui" << endl;
	uint posic = 0;
	for(uint i = 0; i < 16; i++){
		for(uint j = 0; j < 4; j++){

			Point pontos[4];
			pontos[0] = Point(posic*LAR, (ALT));
			pontos[2] = Point(posic*LAR+(LAR-1), ALT-round(vec[j][i]*LAR));
			pontos[1] = Point(posic*LAR+(LAR-1), (ALT));
			pontos[3] = Point(posic*LAR, ALT-round(vec[j][i]*LAR));
			posic++;
			const Point* elementPoints[4] = { &pontos[0], &pontos[1], &pontos[2], &pontos[3] };
			int p_size = 4;
			fillPoly(image, elementPoints, &p_size, 1, Scalar(255, 0, 0), 8);
			if(j == 0){
				fillPoly(image, elementPoints, &p_size, 1, Scalar(100, 100, 100), 8);
			}
			if(j == 1){
				fillPoly(image, elementPoints, &p_size, 1, Scalar(200, 20, 20), 8);
			}
			if(j == 2){
				fillPoly(image, elementPoints, &p_size, 1, Scalar(20, 200, 20), 8);
			}
			if(j == 3){
				fillPoly(image, elementPoints, &p_size, 1, Scalar(20, 20, 200), 8);
			}
		}
		cout << "aqui2" << endl;
		posic++;
	}
	imshow("teste", image);
	imwrite("./results/acu_less_tracks.jpg", image);
	waitKey(0);


    return 0;
}
