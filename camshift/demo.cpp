#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main(){

    Mat img = imread("/home/wxc/Downloads/image.jpg");
    Mat img_1 = imread("/home/wxc/Downloads/images.jpg");
    std::vector<KeyPoint> kp;
    std::vector<KeyPoint> kp_1;
    // Default parameters of ORB
    int nfeatures=2500;
    float scaleFactor=1.2f;
    int nlevels=8;
    int edgeThreshold=15; // Changed default (31);
    int firstLevel=0;
    int WTA_K=2;
    int scoreType=ORB::HARRIS_SCORE;
    int patchSize=31;
    int fastThreshold=20;

    Ptr<ORB> detector = ORB::create(
    nfeatures,
    scaleFactor,
    nlevels,
    edgeThreshold,
    firstLevel,
    WTA_K,
    scoreType,
    patchSize,
    fastThreshold);

    Ptr<ORB> detector_2 = ORB::create(
    nfeatures,
    scaleFactor,
    nlevels,
    edgeThreshold,
    firstLevel,
    WTA_K,
    scoreType,
    patchSize,
    fastThreshold );
//
    Ptr<DescriptorMatcher> matcher =DescriptorMatcher::create("BruteForce-Hamming");
    Ptr<DescriptorExtractor> descriptorExtractor = ORB::create();;
//match
    BFMatcher matche(NORM_L2,true);
    vector <vector<DMatch> >matches;
//    vector<Point2f> featurePoints1;
//    vector<Point2f> featurePoints2;
    Mat queryDescriptor,queryDescriptor_1;
//1
    detector->detect(img, kp);
    descriptorExtractor->compute(img,kp,queryDescriptor);
//2
    detector_2->detect(img_1,kp_1);
    descriptorExtractor->compute(img_1,kp_1,queryDescriptor_1);
//match
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
    Mat output;
    drawMatches(img,kp,img_1,kp_1,m_Matches,output,Scalar::all(-1),CV_RGB(255,255,255),Mat(),2);
    std::cout << "Found " << kp.size() << " Keypoints " << std::endl;
    imshow("output",output);
//    Mat out,out_2;
//    drawKeypoints(img, kp, out, Scalar::all(-1));
//     drawKeypoints(img_1, kp_1, out_2, Scalar::all(-1));
//    imshow("Kpts", out);
//    imshow("big",out_2);
    waitKey(0);
    return 0;
}
