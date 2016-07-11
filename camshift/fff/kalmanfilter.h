#ifndef KALMANFILTER_H
#define KALMANFILTER_H
#include <opencv2/core.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

class kalmanfilter
{
public:
    kalmanfilter();
    Rect kalmanfilter::setcurrentrect(Rect & ub);
    void kalmanfilter::init(Rect & ub);
    Point getcurrent();
private:
    KalmanFilter km;
    Mat_<float> measurement;
    Mat measurement;
    RNG rng;
    Point kfpredict;
    Point kmCorrectCenter;
};

#endif // KALMANFILTER_H
