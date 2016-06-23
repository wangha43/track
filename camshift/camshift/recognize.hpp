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
#include <vector>
#include <iostream>
#include "detect.hpp"
using namespace std;
class recognizer{
public :
    recognizer();
    vector<vector<cv::Point2f> > get_orbpoints_array;
private:
    vector<vector<cv::Point2f> > orbpoints_array;
};

#endif /* recognize_hpp */
