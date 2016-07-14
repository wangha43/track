#include "kalmanfilter.h"

kalmanf::kalmanf()
{
}

kalmanf::~kalmanf()
{

}
void kalmanf::init(Rect ub){
    const int stateNum = 4;
    const int measureNum = 2;
    km =KalmanFilter(stateNum,measureNum,0);
    Mat statePost= (Mat_<float>(4, 1) <<ub.x+ub.width/2,ub.y+ub.height/2,0,0);
    Mat transitionMatrix = (Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);
    km.init(stateNum,measureNum);
    km.transitionMatrix = transitionMatrix;
    km.statePost = statePost;
    //init setIdentity
    setIdentity(km.measurementMatrix);
    setIdentity(km.processNoiseCov, Scalar::all(1e-5));
    setIdentity(km.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(km.errorCovPost, Scalar::all(1));
    measurement = Mat::zeros(measureNum, 1, CV_32F);
}

Rect kalmanf::setcurrentrect(Rect2d & ub)
{
    //predict and get predict point
    km.predict();
    kfpredict =getcurrent();
    //set new position
    measurement.at<float>(0) =(float) ub.x+ub.width/2;
    measurement.at<float>(1) =(float) ub.y+ub.height/2;
    //update
    km.correct(measurement);
    kmCorrectCenter = getcurrent();
    ub = Rect2d(kmCorrectCenter.x-ub.width/2,
              kmCorrectCenter.y-ub.height/2,
              ub.width,
              ub.height);
    return Rect2d(kmCorrectCenter.x-ub.width/2,
                kmCorrectCenter.y-ub.height/2,
                ub.width,
                ub.height);
}

Point kalmanf::getcurrent() {
    Mat statePost = km.statePost;
    return Point(statePost.at<float>(0), statePost.at<float>(1));
}
