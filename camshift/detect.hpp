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
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <iostream>
#include "opencv2/video/background_segm.hpp"

using namespace std;
using namespace cv;
class detecter{
public :
    detecter();
    void findarea(cv::Mat & a);
    Ptr<BackgroundSubtractorMOG2> bgsubtractor;
    vector<Rect> get_area();
private:
    vector<Rect> detected_area;
};

#endif /* detect_hpp */
