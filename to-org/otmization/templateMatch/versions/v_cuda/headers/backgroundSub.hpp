#ifndef BACKGROUNG_SUB
#define BACKGROUNG_SUB

class Background{

public:
    Background(Mat, Mat);
    void alloc2Image();
    void allocImage();
    void tranferImage();

    void runkernellSub();
    void getDiffentsPoints();


}

#endif
