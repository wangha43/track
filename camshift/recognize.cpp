//
//  recognize.cpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//

#include "recognize.hpp"

recognizer::recognizer(){
    detector=FastFeatureDetector::create(15);
    extractor= SURF::create();
}
void recognizer::setThrehold(int a){
    threholdNum =a;
}

bool recognizer::getmatched( Mat  mat1, Mat  mat2){
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    detector->detect(mat1,keypoints_object);
    detector->detect(mat2,keypoints_scene);
    if(keypoints_object.size()==0 || keypoints_scene.size()==0){
        return false;
    }
    Mat descriptors1, descriptors2;
    extractor->compute(mat1, keypoints_object, descriptors1);
    extractor->compute(mat2, keypoints_scene, descriptors2);
    BFMatcher matcher;
    vector<DMatch> matches;

    matcher.match(descriptors1, descriptors2, matches);
    if(matches.size()<threholdNum)
        return false;
    return true;
}
