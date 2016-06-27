#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"
#include "opencv2/video/tracking.hpp"

#include "camshift.hpp"
#include "detect.hpp"
#include "recognize.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
vector<camshifttracker> camtracker;
vector<KeyPoint> keyPoint_1;
 bool tracker_sys=false;
 bool moving = false;
Rect boundRect;
Mat mat_of_first;
vector<int> count_time;
void vesusmatch(Mat mat1,vector<Rect> & vec1,Mat mat2,vector<Rect> & vec2, recognizer * rec){
        if(vec1.size()!=0 && vec2.size()!= 0){
            vector<Rect> vect1(vec1);
            vector<Rect> vect2(vec2);
        for(vector<Rect>::iterator iter1 = vect1.begin(); iter1!=vect1.end();){
            bool matched = false;
            int k = distance(vect1.begin(),iter1);
            for(vector<Rect>::iterator iter2 = vect2.begin();iter2!=vect2.end();){
                 bool match = rec->getmatched(Mat(mat1,*iter1),Mat(mat2,*iter2));
                 if(match){
                       count_time[k] = 0;
                       vect2.erase(iter2);
                       matched = true;
                       break;
                 }else{
                    iter2++;
                 }
            }
            if(!matched){
                count_time[k]++;
                if(count_time[k]>10){
                    //erase disappear 10 time object and statistics
                    vect1.erase(iter1);
                    vec1.erase(iter1);
                    count_time.erase(count_time.begin()+k);
                    //make it run truely for every vector
                    iter1--;
                }
            }
            cout << matched <<endl;
            iter1++;
        }
        if(vec2.size()>0){
            vec1.insert(vec1.end(),vect2.begin(),vect2.end());
            vector<int> cc(vect2.size(),0);
            count_time.insert(count_time.end(),cc.begin(),cc.end());
        }
     }
        if(vec1.size()==0 && vec2.size()!= 0){
            vec1 = vec2;
            vector<int> cc(vec2.size(),0);
            count_time = cc;
        }
}

vector<Rect> trackedRect;
vector<Rect> foundRect;
int main(){
    VideoCapture cap;
    cap.open(0);
     if( !cap.isOpened() )
    {
        cout << "***Could not initialize capturing...***\n";
        return -1;
    }
    detecter * dt = new detecter;
    recognizer * recogn = new recognizer;
    recogn->setThrehold(5);
    camshifttracker ctracker;
    bool stop =false;
    bool firsttrack = true;
    Mat kframe,bgmask,back_frame,tmp_frame,s_frame;
    while(!stop){
        cap >> s_frame;
            cvtColor(s_frame,tmp_frame,COLOR_BGR2GRAY);
            blur(tmp_frame,kframe,Size(3,3),Point(-1,-1));
            Ptr<BackgroundSubtractorMOG2> bgsubtractor = dt->bgsubtractor;
            bgsubtractor->apply(kframe, bgmask, (true) ? -1 : 0);
            bgsubtractor->getBackgroundImage(back_frame);

            //init the tracked Rect
            if(firsttrack){
                trackedRect = dt->findarea(bgmask);
                if(trackedRect.size()>0){
                    int siz = trackedRect.size();
                    vector<int> k(siz,0);
                    count_time = k;
                }
            }else{
                foundRect = dt->findarea(bgmask);
            }

              if(!firsttrack){
                   vesusmatch(mat_of_first,trackedRect,tmp_frame,foundRect,recogn);
              }
              mat_of_first =tmp_frame;
              firsttrack = false;
              ctracker.setMainImage(s_frame);
              for(size_t i = 0;i<trackedRect.size();i++){
//                    cout<<trackedRect.size()<<endl;
                    ctracker.setCurrentRect(trackedRect[i]);
                    if(ctracker.trackCurrentRect().boundingRect().area() <=1)
                        continue;
                    rectangle(s_frame,ctracker.trackCurrentRect().boundingRect(),cv::Scalar(255, 255, 255));
              }

              //
//                if(tracker_sys == true){

            imshow("video", s_frame);
            imshow("foreground", bgmask);
            imshow("background",back_frame);
            cvWaitKey(1);
    }
    delete [] dt;
    delete [] recogn;
    return 0;
}
