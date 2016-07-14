#ifndef KALMANF_H
#define KALMANF_H
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

class kalmanf
{
public:
    kalmanf();
    ~kalmanf();
    Rect setcurrentrect(Rect2d & ub);
    //init with a rect
    void init(Rect ub);
    Point getcurrent();
private:
    KalmanFilter km;
    Mat_<float> measurement;
    RNG rng;
    Point kfpredict;
    Point kmCorrectCenter;
};

#endif
