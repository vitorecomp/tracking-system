#include "track.h"

Tracking::Tracking(Mat mainImage){
    //pega a imagem
    imshow("Imagem", mainImage);
    waitKey(50);
    nclicks = 0;
    hasclicks = false;
    pthread_mutex_init(&clicks, NULL);

    cout << "Por favor clique no Ponto esquerda cima, e em sequencia no Ponto direita baixo" << endl;
    cout << "Caso contrario o programa sera finalizaado" << endl;
    //seta o mouse
    setMouseCallback("Imagem", mouseHandler, (void*)this);
    //verificando se ja hove dois clicks
    while(1){
      if(this->twoCLicks())
        break;
        imshow("Imagem", mainImage);
        waitKey(50);
    }

    Point size = rightDown - leftUp;
    if(size.x < 0 || size.y < 0){
        cout << "deveria ter clicado no Ponto esquerda cima, e em sequencia no Ponto direita baixo, agora vou finalizar" << endl;
        exit(1);
    }

	//this->templ = Mat(mainImage, Rect(leftUp.x, leftUp.y, size.x, size.y)).clone();

	this->templ = mainImage.clone();
    for(int i = 0; i < templ.cols;i++)
        for(int j = 0; j < templ.rows; j++)
            if(i < leftUp.x || i > rightDown.x || j < leftUp.y || j > rightDown.y){
                templ.at<Vec3b>(j,i)[0] = 0;
                templ.at<Vec3b>(j,i)[1] = 0;
                templ.at<Vec3b>(j,i)[2] = 0;
            }


    rectangle(mainImage, leftUp, rightDown, Scalar( 0, 255, 255 ), 1, 8, 0);
    imshow("Imagem2", templ);
    imshow("Imagem", mainImage);
    waitKey(50);

    this->iframe = mainImage.clone();

    // vector of keypoints
    Mat featureImage;
    // Construct the SURF feature detector object
    this->hold = 1500.;
    while(1){
        cv::SurfFeatureDetector surf(hold); // threshold
    // Detect the SURF features
        surf.detect(templ,keypoints);
        if(keypoints.size() < 20)
            hold -= 10;
        else
            break;
    }
    std::cout << "Computando com hold de:" << hold << std::endl;
    cv::drawKeypoints(templ, keypoints, featureImage, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    surfDesc.compute(templ,keypoints,descriptors);
    imshow("ImagemL", featureImage);
    waitKey(50);
}

void Tracking::click(int x, int y){
  pthread_mutex_lock(&clicks);
  if(nclicks == 0)
    leftUp = Point(x, y);
  else
    rightDown = Point(x, y);
  nclicks++;
  if(nclicks == 2){
    hasclicks = true;
    nclicks = 0;
  }
  pthread_mutex_unlock(&clicks);
}

bool Tracking::twoCLicks(){
  pthread_mutex_lock (&clicks);
  bool has = hasclicks;
  pthread_mutex_unlock (&clicks);
  return has;
}

void mouseHandler(int event, int x, int y, int flags, void *param)
{
  Tracking *track = (Tracking*)param;
  //cout << x << " " << y << " " << flags <<  " " << event<< endl;
  if(event == 1){
    track->click(x, y);
  }
}

void Tracking::process(Mat frame){
    Mat featureImage;
    vector<cv::KeyPoint> keypoints2;
	Mat descriptors2;
    SurfFeatureDetector surf(hold*1.7); // threshold

    // Detect the SURF features
    surf.detect(frame,keypoints2);
    cv::drawKeypoints(frame, keypoints2, featureImage, cv::Scalar(255,255,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    surfDesc.compute(frame,keypoints2,descriptors2);
    imshow("ImagemN", featureImage);
    waitKey(50);

    cv::BruteForceMatcher<cv::L2<float> > matcher;
    // Match the two image descriptors
    std::vector<cv::DMatch> matches;
    matcher.match(descriptors,descriptors2, matches);

    /*
    std::nth_element(matches.begin(), // initial position
    matches.begin()+24, // position of the sorted element
    matches.end()); // end position
    // remove all elements after the 25th
    matches.erase(matches.begin()+25, matches.end());
    */
    cv::Mat imageMatches;
    cv::drawMatches(
    templ,keypoints, // 1st image and its keypoints
    frame,keypoints2, // 2nd image and its keypoints
    matches, // the matches
    imageMatches, // the image produced
    cv::Scalar(255,255,255)); // color of the lines
    imshow("ImagemFinal", imageMatches);
    waitKey(50);

}
