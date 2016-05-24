#include <vector>
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>


using namespace cv;
using namespace std;
#define ullint unsigned long long int



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
    if(argc < 3){
        cout << "Erro de argumentos:" << endl;
        cout << "Utilização de : ./result origem saida" << endl;
        return -1;
    }

    string videoName = argv[1];
    string outName = argv[2];


    ///////////////////////////////////////////
    //Preparando
    //video
    VideoCapture cap(videoName.c_str()); // open the video file for reading
    if (!cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //fps do video -- partiu tentar tempo real
    double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

    //saida
    int ex = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
    std::cout <<  cap.get(CV_CAP_PROP_FPS) << std::endl;
    Size S = Size((int) cap.get(CV_CAP_PROP_FRAME_WIDTH) * 4,    // Acquire input size
              (int) cap.get(CV_CAP_PROP_FRAME_HEIGHT)*4);
    VideoWriter outputVideo;                                        // Open the output
    outputVideo.open(outName.c_str(), ex, cap.get(CV_CAP_PROP_FPS), S, true);
    if (!outputVideo.isOpened()){
     cout  << "Could not open the output video for write: "<< endl;
     return -1;
    }

    //abrindo janelas de exibiçao
    Mat frame;
    Mat frameBig;
    while(1)
    {
        //pegando o quadro
        bool bSuccess = cap.read(frame);
        if (!bSuccess){
            //cout << "Cannot read the frame from video file" << endl;
            break;
        }
        frameBig = Mat::zeros(frame.rows*4, frame.cols*4, CV_8UC3);
        for(int i = 0; i < frame.rows; i++)
            for(int j = 0; j < frame.cols; j++)
                for(int x = 0; x < 4; x++)
                    for(int y = 0; y < 4; y++)
                        for(int c = 0; c < 3; c++)
                            frameBig.at<Vec3b>(i*4 + x, j*4 + y)[c] = frame.at<Vec3b>(i, j)[c];

        //cout << "Frame x" << endl;
        imshow("FrameBig", frameBig);
        waitKey(10);

        outputVideo << frameBig;
     }
     return 0;
}
