//
//  recognize.cpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//

#include "recognize.hpp"

recognizer::recognizer(){
    detector=FastFeatureDetector::create(0.8);
    extractor= SURF::create();
}
void recognizer::setThrehold(int a){
    threholdNum =a;
}

bool recognizer::getmatched(  Mat  mat1, Mat  mat2){
    Mat det1=mat1;Mat det2 = mat2;
    std::vector<KeyPoint> keypoints_object, keypoints_scene;
    detector->detect(det1,keypoints_object);
    detector->detect(det2,keypoints_scene);
    if(keypoints_object.size()==0 || keypoints_scene.size()==0){
        return false;
    }
    Mat descriptors1, descriptors2;
    extractor->compute(det1, keypoints_object, descriptors1);
    extractor->compute(det2, keypoints_scene, descriptors2);
    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);
//    Rect r3 = det1&det2;
//    double match = r3.area()/det2.area();
    if(matches.size()<threholdNum)
        return false;
    return true;
}

void recognizer::vesusmatch(Mat & mat1,vector<int> & count_time,detecter * de){
//    cout <<"vec1:"<<vec1.size()<<endl;
//    cout <<"vec2:"<<vec2.size()<<endl;
//    vector<Rect> vect1(vec1);
//    vector<Rect> vect2(vec2);
//    if(vect1.size()!=0 ){
//        //track new found area
//        if(vect2.size()!=0){
//            bool match = getmatched(Mat(mat1,vect1[0]),Mat(mat2,vect2[0]));
//            if(match){
//                count_time[0]=0;
//            }else{
//                count_time[0]++;
//            }
//        }else{
//            count_time[0]++;
//        }
//    } else if(vect1.size()==0 && vect2.size()!= 0){
//        vec1.insert(vec1.end(),vect2.begin(),vect2.end());
//        vector<int> cc(vect2.size(),0);
//        count_time.insert(count_time.end(),cc.begin(),cc.end());;
//    }

        vector<Rect> found_body;
        de->upperbody_detector.detectMultiScale(mat1,found_body);
        if(found_body.size()==0){
            int a = ++count_time[0];
            count_time.clear();
            count_time.push_back(a);
        }else{
            count_time.clear();
            count_time.push_back(0);
        }
        cout<<found_body.size()<<endl;

}
