//
//  detect.cpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//

#include "detect.hpp"

detecter::detecter(){
    bgsubtractor = createBackgroundSubtractorMOG2();
    bgsubtractor->setVarThreshold(20);
    bgsubtractor->setDetectShadows(true);
    bgsubtractor->setShadowValue(0);
}

void detecter::findarea(cv::Mat & a){

}
vector<Rect> detecter::get_area(){
    return detected_area;
}

