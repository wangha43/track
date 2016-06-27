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

detecter * dt = new detecter;
recognizer * recogn = new recognizer;
camshifttracker ctracker;
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
    recogn->setThrehold(5);
    
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
                    size_t siz = trackedRect.size();
                    vector<int> k(siz,0);
                    count_time.insert(count_time.end(),k.begin(),k.end());
                }
            }else{
                foundRect = dt->findarea(bgmask);
            }

              if(!firsttrack){
                 recogn->vesusmatch(mat_of_first,trackedRect,tmp_frame,foundRect,count_time);
              }
              mat_of_first = tmp_frame;
              firsttrack = false;
        
              for(size_t i = 0;i<trackedRect.size();i++){
//                    cout<<trackedRect.size()<<endl;
                  
                   ctracker.setMainImage(s_frame);
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
            waitKey(200);
    }
    delete dt;
    delete recogn;
    return 0;
}
