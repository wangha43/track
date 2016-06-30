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

bool recognizer::getmatched( Rect  r1, Rect  r2){
    Rect det1=r1;Rect det2 = r2;
//    std::vector<KeyPoint> keypoints_object, keypoints_scene;
//    detector->detect(det1,keypoints_object);
//    detector->detect(det2,keypoints_scene);
//    if(keypoints_object.size()==0 || keypoints_scene.size()==0){
//        return false;
//    }
//    Mat descriptors1, descriptors2;
//    extractor->compute(det1, keypoints_object, descriptors1);
//    extractor->compute(det2, keypoints_scene, descriptors2);
//    BFMatcher matcher;
//    vector<DMatch> matches;
//    matcher.match(descriptors1, descriptors2, matches);
    Rect r3 = det1&det2;
    double match = r3.area()/det2.area();
    if(match<threholdNum)
        return false;
    return true;
}

void recognizer::vesusmatch(Mat & mat1,vector<Rect> & vec1,Mat & mat2,vector<Rect> & vec2,vector<int> & count_time){
    cout <<"vec1:"<<vec1.size()<<endl;
    cout <<"vec2:"<<vec2.size()<<endl;
    vector<Rect> vect1(vec1);
    vector<Rect> vect2(vec2);
    if(vect1.size()!=0 ){
        //track new found area
        for(vector<Rect>::iterator iter1 = vect1.begin(); iter1!=vect1.end();){
            bool matched = false;
            size_t k = distance(vect1.begin(),iter1);
            Rect rect1 = *iter1;
            for(vector<Rect>::iterator iter2 = vect2.begin();iter2!=vect2.end();){
                Rect rect2 = *iter2;
                bool match = getmatched(rect1,rect2);
                if(match){
                    count_time[k] = 0;
                    vect2.erase(iter2);
                    matched = true;
                }else{
                    iter2++;
                }
            }
            //new area not found area
/*            if(!matched){
//                bool matchall = getmatched(Mat(mat1,rect1),Mat(mat2));
//                if(!matchall){
                     count_time[k]++;
//                }else{
//                      count_time[k]=0;
//                }
                if(count_time[k]>3){
                    //erase disappear 10 time object and statistics
                    vect1.erase(iter1);
                    vec1.erase(vec1.begin()+k);
                    count_time.erase(count_time.begin()+k);
                    //make it run truely for every vector
                    iter1--;
                }
            }   */
            iter1++;
        }
        if(vect2.size()>0){
            //add new found area to tracked area
            vec1.insert(vec1.end(),vect2.begin(),vect2.end());
            vector<int> cc(vect2.size(),0);
            count_time.insert(count_time.end(),cc.begin(),cc.end());
        }
    } else if(vect1.size()==0 && vect2.size()!= 0){
        vec1.insert(vec1.end(),vect2.begin(),vect2.end());
        vector<int> cc(vect2.size(),0);
        count_time.insert(count_time.end(),cc.begin(),cc.end());;
    }

}
