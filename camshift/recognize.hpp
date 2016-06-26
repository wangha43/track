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
#include <opencv2/features2d.hpp>
#include <vector>
#include <iostream>
#include "detect.hpp"
using namespace std;
class recognizer{
public :
    recognizer();
    //查看两个图片匹配不
    bool getmatched(const Mat & mat1,const Mat & mat2);
    //设置匹配点阈值 小于匹配点数量则认为不匹配
    void setThrehold(int a);
private:
    Ptr<DescriptorExtractor> descriptorExtractor;
    Ptr<ORB> detector;
    Ptr<DescriptorMatcher> matcher;
    int threholdNum;
};

#endif /* recognize_hpp */
