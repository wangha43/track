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

//Ptr<BackgroundSubtractorMOG2> getSubstractor(){
//    return bgsubtractor;
//}

vector<Rect> detecter::findarea(cv::Mat & a){
    vector<Rect> result;
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp;
    Mat element;
    
    element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
    dilate(a, temp, element, Point(4,4), niters);
    erode(temp, temp, element, Point(4,4), niters*2);
    dilate(temp, temp, element, Point(4,4), niters*3);
    threshold(temp, temp, 128, 255, CV_THRESH_BINARY);
    imshow("temp",temp);
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    for(size_t i= 0;i<contours.size();i++){
        double a = contourArea(contours[i]);
        if(a<9){
            break;
        }
        result.push_back(boundingRect(contours[i]));
    }
    return result;
}
