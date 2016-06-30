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
    upperbody_detector.load("haarcascade_upperbody.xml");
}

//Ptr<BackgroundSubtractorMOG2> getSubstractor(){
//    return bgsubtractor;
//}

vector<Rect> detecter::findarea(cv::Mat & a,Mat & gray){
    vector<Rect> result;
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp;
    Mat element;

    element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
    dilate(a, temp, element, Point(3,3), niters);
    erode(temp, temp, element, Point(3,3), niters);
    dilate(temp, temp, element, Point(3,3), niters);
    threshold(temp, temp, 128, 255, CV_THRESH_BINARY);
    imshow("temp",a);
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );

    for(size_t i= 0;i<contours.size();i++){
        double b = contourArea(contours[i]);
        if(b<3){
           continue;
        }
        Rect are = boundingRect(contours[i]);
        vector<Rect> found_body;
        upperbody_detector.detectMultiScale(Mat(gray,are),found_body);
        if(found_body.size()==0){
            continue;
        }else{
            for(size_t i = 0;i<found_body.size();i++){
                Rect bod = Rect(found_body[i].x+are.x,found_body[i].y+are.y,found_body[i].width,found_body[i].height);
                result.push_back(bod);
//                rectangle(temp,bod,Scalar(255,255,255),5);
            }
        }

    }

    return result;
}
