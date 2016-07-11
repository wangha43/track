#include "kalmanfilter.h"

kalmanfilter::kalmanfilter()
{
}

void kalmanfilter::init(Rect & ub){
    km =KalmanFilter(4,2,0);
    Mat statePost= (Mat_<float>(4, 1) <<ub.x+ub.width/2,ub.y+ub.height/2,0,0);
    Mat transitionMatrix = *(Mat_<float>(4, 4) <<1,0,1,0,0,1,0,1,0,0,1,0,0,0,0,1);
    km.init(statePost,transitionMatrix);
    KF.transitionMatrix = transitionMatrix;
    KF.statePost = statePost;
    //init setIdentity
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
    setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
    setIdentity(KF.errorCovPost, Scalar::all(1));

    measurement = Mat::zeros(measureNum, 1, CV_32F);
}

Rect kalmanfilter::setcurrentrect(Rect & ub)
{
    //predict and get predict point
    km.predict();
    kfpredict =getcurrent();
    //set new position
    measurement.at<float>(0) =(float) ub.x+ub.width/2;
    measurement.at<float>(1) =(float) ub.y+ub.height/2;
            //update
    KF.correct(measurement);
    kmCorrectCenter = getCurrentState();
    return Rect(kmCorrectCenter.x-ub.width/2,
                kmCorrectCenter.y-ub.height/2,
                ub.width,
                ub.height);
}

Point getcurrent(){
    Mat statePost = KF.statePost;
    return Point(statePost.at<float>(0), statePost.at<float>(1));
}
