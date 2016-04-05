#include <vector>
#include <highgui.h>
#include <cv.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <sys/time.h>
#include <dirent.h>


using namespace cv;
using namespace std;
#define ullint unsigned long long int

int main(int argc, char *argv[])
{
    //argumentos
    if(argc < 3){
        cout << "Erro de argumentos:" << endl;
        cout << "Utilização de : ./result folder saida" << endl;
        return -1;
    }

    string folder = argv[1];
	string outName_t = argv[2];
    string outName = folder + outName_t;



    ///////////////////////////////////////////
    //fazendo a lista de frames
	std::vector<string> names;
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (folder.c_str())) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
	    string name_inter = ent->d_name;
		if(name_inter.substr(name_inter.find_last_of(".") + 1) == "png")
	  		names.push_back(folder + name_inter);
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("Porra");
	  return EXIT_FAILURE;
	}

    for(int i = 0; i < names.size(); i++){
        for(int j = i+1; j < names.size(); j++){
            if(strcasecmp(names[i].c_str(), names[j].c_str()) >= 0){
                string inter = names[i];
                names[i] = names[j];
                names[j] = inter;
            }
        }
    }

    for(int i = 0; i < names.size(); i++){
        std::cout << names[i] << std::endl;
    }

    //pegando o primeiro frame
    Mat frame;
	frame = imread(names[0].c_str(), CV_LOAD_IMAGE_COLOR);
	imshow("Teste", frame);
	waitKey(20);
	if (!frame.data){
		exit(1);
	}
	int width, heigh;

	//abrindo a saida
    int ex = 845639789;     // Get Codec Type- Int form
    Size S = Size(frame.cols,frame.rows);

    VideoWriter outputVideo;                                        // Open the output
    outputVideo.open(outName.c_str(), ex, 25, S, true);
    if (!outputVideo.isOpened()){
     cout  << "Could not open the output video for write: "<< endl;
     return -1;
    }

    //abrindo janelas de exibiçao
	outputVideo << frame;
    for(int i = 1; i < names.size();i++){
		frame = imread(names[i].c_str(), CV_LOAD_IMAGE_COLOR);
        if (!frame.data){
            break;
        }
        imshow("FrameBig", frame);
        waitKey(20);
    	outputVideo << frame;
     }
     return 0;
}
