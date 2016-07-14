//
//  detect.hpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//
//TO DO GMM

#ifndef DETECT_H
#define DETECT_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include <string>
#include "opencv2/video/background_segm.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/objdetect.hpp"

using namespace std;
using namespace cv;
class detecter{
public :
    detecter();
    Rect findarea(cv::Mat & a,Mat & gray,Mat & frame,Rect2d & traking);
//    Ptr<BackgroundSubtractorMOG2> getSubstractor();
    Ptr<BackgroundSubtractorMOG2> bgsubtractor;
    CascadeClassifier upperbody_detector;

};

#endif /* detect_hpp */
