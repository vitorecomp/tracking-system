#ifndef TRACKLETS
#define TRACKLETS

class Tracklet{
public:
    enum METRIC {ACURACY, TIME};
    enum METHOD {TEMPLATE_MACTHING, SIFT, SURF, BACKGROUND_SUBTRACTION};

    void setMetric(METRIC);
    void setMethod(METHOD)
};

#endif
