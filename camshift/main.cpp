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

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
vector<camshifttracker> camtracker;
vector<KeyPoint> keyPoint_1;
 bool tracker_sys=false;
 bool moving = false;
 Rect boundRect;
static void refineSegments(Mat& img, Mat& mask, Mat& dst)
{
    int niters = 3;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat temp;
    Mat element;

     //9*9 area
    element = getStructuringElement(MORPH_RECT, Size(9, 9), Point(-1, -1));
    dilate(mask, temp, element, Point(4,4), niters);
    erode(temp, temp, element, Point(4,4), niters*2);
    dilate(temp, temp, element, Point(4,4), niters*3);
    threshold(temp, temp, 128, 255, CV_THRESH_BINARY);
    //turn into binary image
    findContours( temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
   // dst = Mat::zeros(img.size(), CV_8UC3);
    if( contours.size() == 0){
        moving = false;
        tracker_sys = false;
        return;
}
    tracker_sys = true;
    vector<vector<Point> > contours_poly(contours.size());
    vector<Rect_<int> >  faceshoulders;
    int largest_area = 0;
    int largest_area_index=0;
    for(int i= 0;i<contours.size();i++){
         double a = contourArea(contours[i]);
        if(a>largest_area){
            largest_area=a;
            largest_area_index = i;
        }

    }
    camshifttracker ctracker;
   
    ctracker.setCurrentRect(boundingRect(contours[largest_area_index]));
    cout << boundingRect(contours[largest_area_index]).width<<endl;
    camtracker.push_back(ctracker);
     boundRect=boundingRect(contours[largest_area_index]);
//     Scalar color = Scalar(255,255,255);
     Mat tmp_rect = img(Rect(boundRect.x,boundRect.y,boundRect.width,boundRect.height)).clone();
  Ptr<SURF> detector = SURF::create(400);
     detector->detect(tmp_rect,keyPoint_1);

     if(keyPoint_1.size()!=0){
         Mat draw ;
         drawKeypoints( tmp_rect, keyPoint_1, draw, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
         imshow("draw",draw);
     }
}

//static void track(Rect & boundRect,Mat & img){
//    if(tracker_sys){
//       Mat hsv,mask,hue,hist,histimg,backproj;
//       int hsize = 16;
//       Rect trackwindow;
//       float hranges[] = {0,180};//hranges在后面的计算直方图函数中要用到
//       const float* phranges = hranges;
//       cvtColor(img,hsv,CV_GRAY2BGR);
//       cvtColor(hsv,hsv,CV_BGR2HSV);
//       int vmin = 10, vmax = 256, smin = 30;
//       int _vmin = vmin, _vmax = vmax;
//       inRange(hsv, Scalar(0, smin, MIN(_vmin,_vmax)),Scalar(180, 256, MAX(_vmin, _vmax)), mask);
//       hue.create(hsv.size(),hsv.depth());
//       int ch[] = {0,0};
//       mixChannels(&hsv,1,&hue,1,ch,1);
//       Mat roi(hue,boundRect),maskroi(mask,boundRect);
//       calcHist(&roi, 1, 0, maskroi, hist, 1, &hsize, &phranges);
//       normalize(hist, hist, 0, 255, CV_MINMAX);
//       trackwindow = boundRect;

//         histimg = Scalar::all(0);
//         int binW = histimg.cols / hsize;
//         Mat buf(1, hsize, CV_8UC3);
//          for( int i = 0; i < hsize; i++ )
//               buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
//         cvtColor(buf, buf, CV_HSV2BGR);
//       histimg = Scalar::all(0);
//       int binW = histimg.cols / hsize;
//       Mat buf(1, hsize, CV_8UC3);
//       for( int i = 0; i < hsize; i++ )
//           buf.at<Vec3b>(i) = Vec3b(saturate_cast<uchar>(i*180./hsize), 255, 255);
//       cvtColor(buf, buf, COLOR_HSV2BGR);

//       for( int i = 0; i < hsize; i++ )
//       {
//           int val = saturate_cast<int>(hist.at<float>(i)*histimg.rows/255);
//           rectangle( histimg, Point(i*binW,histimg.rows),
//           Point((i+1)*binW,histimg.rows - val),
//           Scalar(buf.at<Vec3b>(i)), -1, 8 );
//       }
//        calcBackProject(&hue, 1, 0, hist, backproj, &phranges);
//        backproj &= mask;
//        RotatedRect trackBox = CamShift(backproj, trackwindow,
//                                cv::TermCriteria( CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 10, 1 ));
//        cout <<"tracking"<<endl;
//        Rect rRect = trackBox.boundingRect();
//        rectangle(img,rRect,Scalar(255,255,255));
//        Mat rr = img(Rect(rRect.x,rRect.y,rRect.width,rRect.height)).clone();
//        cout <<rRect.x<<"-> x"<<endl<<rRect.width<<"->y"<<endl;
//        imshow ("RRRR",rr);
//    }
//}

int main(){
    VideoCapture cap;
    cap.open(0);
     if( !cap.isOpened() )
    {
        cout << "***Could not initialize capturing...***\n";
        return -1;
    }
           detecter * dt = new detecter;
    bool stop =false;
      Mat kframe,bgmask,back_frame,tmp_frame,s_frame;
    while(!stop){
        cap >> s_frame;
        cvtColor(s_frame,tmp_frame,COLOR_BGR2GRAY);
         blur(tmp_frame,kframe,Size(3,3),Point(-1,-1));

            dt->bgsubtractor->apply(kframe, bgmask, (true) ? -1 : 0);
            dt->bgsubtractor->getBackgroundImage(back_frame);

            refineSegments(tmp_frame, bgmask, kframe);
//            cout <<camtracker.size()<<endl;
            for(int i=0; i<camtracker.size(); i++)
            {
                camtracker[i].setMainImage(s_frame);
                cout <<"rect:"<<camtracker[i].getCurrentRect().width<<endl;
                cout<<"area:"<<camtracker[i].trackCurrentRect().boundingRect().area()
                   <<endl;
                if(camtracker[i].trackCurrentRect().boundingRect().area() <= 1)
                    continue;
               ellipse(tmp_frame, camtracker[i].trackCurrentRect(), cv::Scalar(255, 255, 255));
               rectangle(tmp_frame, camtracker[i].trackCurrentRect().boundingRect(), cv::Scalar(255, 255, 255));
            }
  //
//                if(tracker_sys == true){

            imshow("video", tmp_frame);
            imshow("foreground", bgmask);
            imshow("background",back_frame);
            cvWaitKey(1);
    }

    return 0;
}
