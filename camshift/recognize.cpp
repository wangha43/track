//
//  recognize.cpp
//  surf
//
//  Created by wxc on 16/6/22.
//  Copyright © 2016年 wxc. All rights reserved.
//

#include "recognize.hpp"

recognizer::recognizer(){
    int nfeatures=2500;
    float scaleFactor=1.2f;
    int nlevels=8;
    int edgeThreshold=15; // Changed default (31);
    int firstLevel=0;
    int WTA_K=2;
    int scoreType=ORB::HARRIS_SCORE;
    int patchSize=31;
    int fastThreshold=20;
    
    detector = ORB::create(
                                    nfeatures,
                                    scaleFactor,
                                    nlevels,
                                    edgeThreshold,
                                    firstLevel,
                                    WTA_K,
                                    scoreType,
                                    patchSize,
                                    fastThreshold);
    matcher =DescriptorMatcher::create("BruteForce-Hamming");
    descriptorExtractor = ORB::create();
    
}
void recognizer::setThrehold(int a){
    threholdNum =a;
}

bool recognizer::getmatched(const Mat & mat1,const Mat & mat2){
    std::vector<KeyPoint> kp;
    std::vector<KeyPoint> kp_1;
    BFMatcher matche(NORM_L2,true);
    vector <vector<DMatch> >matches;
    Mat queryDescriptor,queryDescriptor_1;
    
    detector->detect(mat1, kp);
    descriptorExtractor->compute(mat1,kp,queryDescriptor);
    detector->detect(mat2, kp);
    descriptorExtractor->compute(mat2,kp,queryDescriptor);
    
    vector<DMatch>m_Matches;
    const float minRatio = 1.f / 1.5f;
    matcher->knnMatch(queryDescriptor,queryDescriptor_1,matches,2);
    for (size_t i=0; i<matches.size(); i++)
    {
        const cv::DMatch& bestMatch = matches[i][0];
        const cv::DMatch& betterMatch = matches[i][1];
        float distanceRatio = bestMatch.distance / betterMatch.distance;
        if (distanceRatio <minRatio)
        {
            m_Matches.push_back(bestMatch);
        }
    }
    if(m_Matches.size()<threholdNum)
        return false;
    return true;
}