#include <vector>
#include <highgui.h>
#include <cv.h>
#include <vector>

#include "track.h"

using namespace cv;
using namespace std;

int main()
{

    ///////////////////////////////////////////
    //Preparando
    //video
    VideoCapture cap(0); // open the video file for reading
    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the video file" << endl;
         return -1;
    }

    //fps do video -- partiu tentar tempo real
    //double fps = cap.get(CV_CAP_PROP_FPS); //get the frames per seconds of the video

    //abrindo janelas de exibiçao
    //namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"

    ////////////////////////////////////////////
    //Prepando o traking
    //primeiro frame para o track
    Mat frame;
    bool bSuccess = cap.read(frame);
    if (!bSuccess){
        cout << "Cannot read the frame from video file" << endl;
        return 1;
    }

    //mascara para o tracking
    imwrite("image.png", frame);
    Tracking track(frame);
    ///////////////////////////////////////////
    Mat frame2;

    //Tracking Start
    while(1)
    {
        bool bSuccess = cap.read(frame2);
        if (!bSuccess){
            cout << "Cannot read the frame from video file" << endl;
            return 1;
        }
        track.process(frame2);
        imshow("MyVideo", frame2);
        int esc = waitKey(70);
        if (esc == 27)
            break;
     }
    return 0;

}
