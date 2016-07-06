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
    upperbody_detector.load("/home/wxc/opencv-3.1.0/data/haarcascades/haarcascade_frontalface_alt.xml");
}

//Ptr<BackgroundSubtractorMOG2> getSubstractor(){
//    return bgsubtractor;
//}

vector<Rect> detecter::findarea(cv::Mat & a,Mat & gray,Mat & frame){
    vector<Rect> result;
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp;
    Mat element;

    element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
    dilate(a, temp, element, Point(3,3), niters);
    erode(temp, temp, element, Point(3,3), niters*1);
    dilate(temp, temp, element, Point(3,3), niters*1);
     imshow("temp",temp);
    threshold(temp, temp, 128, 255, CV_THRESH_BINARY);
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    size_t largest_index = 0;

    //match largest upper body
    vector<Rect> found_body;
    Mat frame_gray;

    size_t largest_body_index=0;
    //find the area
    if(contours.size()!=0){
        double largest_area = 0.0;
        for(size_t i= 0;i<contours.size();i++){
            double a = contourArea(contours[i]);
            if(a>largest_area){
                largest_area = a;
                largest_index = i;
            }
        }
        //if no area do not try to get face
      equalizeHist(gray, frame_gray);
      upperbody_detector.detectMultiScale(Mat(frame_gray,boundingRect(contours[largest_index])),found_body, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
        if(found_body.size()!=0){
            int largest_body = found_body[0].width*found_body[0].height;
            for(size_t k = 0;k<found_body.size();k++){
                int b = found_body[k].width*found_body[k].height;
                if(b>largest_body){
                    largest_body=b;
                    largest_body_index = k;
                }

            }
//            rectangle(gray,found_body[largest_body_index],Scalar(255,255,255));
        }
    }

    if(contours.size()!=0 && found_body.size()!=0){
        Rect boundrect = boundingRect(contours[largest_index]);
        //get the most largest contour
        Rect body = found_body[largest_body_index];
        Rect body_largest = Rect(body.x+boundrect.x,body.y+boundrect.y,body.width+boundrect.width,body.height+boundrect.height);
        Rect inter = body_largest & boundrect;
        if(inter.width*inter.height > 0){
            result.push_back(inter);
        }
    }
    return result;
}
