#include <vector>
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>

#include "track.h"

using namespace cv;
using namespace std;
#define ullint unsigned long long int

ullint time_to_int(struct timeval start, struct timeval end){
    return 	((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec));
}

struct MatchPathSeparator{
    bool operator()( char ch ) const{
        return ch == '/';
    }
};

string basename( std::string const& pathname ){
    return std::string( std::find_if( pathname.rbegin(), pathname.rend(), MatchPathSeparator() ).base(), pathname.end() );
}

int main(int argc, char *argv[])
{
    //argumentos
    if(argc < 7){
        cout << "Erro de argumentos:" << endl;
        cout << "Utilização de : ./result vide pontoOrigemX pontoOrigemY win_x win_y windowProp" << endl;
        cout << "ou utilização de : ./result video pontoOrigemX pontoOrigemY win_x win_y winDowProp -d" << endl;
        return -1;
    }

    string videoName = argv[1];
    Point objCenter(atoi(argv[2]), atoi(argv[3]));
    Point objSize(atoi(argv[4]), atoi(argv[5]));
    double winProp = (double)(atoi(argv[6]))/100;
    bool debug = false;
    bool debug2 = false;

    if (argc == 8){
        string s_debug  = argv[7];
        debug = s_debug.compare("-d") == 0;
        debug2 = s_debug.compare("-a") == 0;
    }

    ///////////////////////////////////////////
    //Preparando
    //video
    VideoCapture cap(videoName.c_str()); // open the video file for reading
    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //fps do video -- partiu tentar tempo real
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

    //abrindo janelas de exibiçao
    if(debug)
        namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    ////////////////////////////////////////////
    //Prepando o traking
    //primeiro frame para o track
    Mat frame;
    bool bSuccess = cap.read(frame);
    if (!bSuccess){
        cout <<  "Cannot read the frame from video file" << endl;
        return 1;
    }

    //mascara para o tracking
    Tracking track(fps, frame, objCenter, objSize, winProp, debug, false);
    //Result result()
    ///////////////////////////////////////////
    Mat frame_u;

    //variaveis temporais
    ullint total = 0;
    struct timeval start, end;

    //Tracking Start
    int i = 0;
    while(1)
    {
        //pegando o quadro
        bool bSuccess = cap.read(frame_u);
        if (!bSuccess){
            //cout << "Cannot read the frame from video file" << endl;
            break;
        }
        if(debug)
            cout << "Frame " << i << endl;
        //prcessando e pegando o tempo de medida
        gettimeofday(&start, NULL);
        track.process(frame_u);
        gettimeofday(&end, NULL);
        total += time_to_int(start, end);
        if(debug2 || debug)
            cout << time_to_int(start, end) << endl;
        //salvar em um log as posições

        //ostranto os debug
        if(true){
            imshow("MyVideo", frame_u);
            waitKey(20);
        }
     }
     printf("%llu\n", total);
     return 0;
}
