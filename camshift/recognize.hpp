//
//  recognize.hpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//
//TODO recogniza with orb

#ifndef RECOGNIZE_H
#define RECOGNIZE_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <vector>
#include <iostream>

#include "detect.hpp"
using namespace std;
using namespace cv::xfeatures2d;
class recognizer{
public :
    recognizer();
    //查看两个图片匹配不
    bool getmatched( Mat mat1, Mat  mat2);
    //设置匹配点阈值 小于匹配点数量则认为不匹配
    void setThrehold(int a);
    void vesusmatch(Mat & mat1,vector<Rect> & vec1,Mat & mat2,vector<Rect> & vec2, vector<int> & count_time);
private:
    Ptr<SURF> extractor;
    Ptr<FeatureDetector> detector;
    size_t threholdNum;
};

#endif /* recognize_hpp */
