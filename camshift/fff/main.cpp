#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <iostream>
#include <vector>
#include <QDateTime>
#include <QDebug>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "detect.hpp"
#include "camshift.hpp"
#include "recognize.hpp"
#include <opencv2/tracking.hpp>
#include "tldtracker.hpp"
#include <opencv2/core/utility.hpp>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

using namespace std;
using namespace cv;

vector<camshifttracker> camtracker;
vector<KeyPoint> keyPoint_1;
vector<Rect> rectang;
vector<bool> recieved;
camshifttracker ctracker;

Rect trackedtrace;
size_t status=0;

bool tracker_sys=false;
bool moving = false;
Rect boundRect;
Mat mat_of_first;
int count_time;
bool tracked = false;

detecter * dt = new detecter;
recognizer * recogn = new recognizer;
tldtracker * tl = new tldtracker;
vector<Rect> trackedRect;
Rect foundRect;
bool firsttrack = true;
Ptr<Tracker>  ptracker =Tracker::create("KCF");
Mat s_frame,kframe,bgmask,back_frame,tmp_frame,mat_track;
Ptr<BackgroundSubtractorMOG2> bgsubtractor = createBackgroundSubtractorMOG2();
static Rect2d rec;
void track_it(Mat  s_frame){

    cvtColor(s_frame,tmp_frame,COLOR_BGR2GRAY);
    blur(tmp_frame,kframe,Size(3,3),Point(-1,-1));
    Ptr<BackgroundSubtractorMOG2> bgsubtractor = dt->bgsubtractor;
    bgsubtractor->apply(kframe, bgmask, (true) ? -1 : 0);
    bgsubtractor->getBackgroundImage(back_frame);
    imshow("bg",bgmask);
    //init find Rect

    foundRect = dt->findarea(bgmask,tmp_frame,s_frame);

    //track with comparison
    if(tracked){
        if(trackedtrace.width*trackedtrace.height < foundRect.width*foundRect.height){
            trackedtrace=foundRect;
            mat_track = s_frame;
            firsttrack = true;
            tracked = true;
        }
    }
    cout<<"tracked:"<<tracked<<endl;
    // track the founded
    if(trackedtrace.width*trackedtrace.height == 0 && foundRect.width*foundRect.height!=0){
        trackedtrace=foundRect;
        mat_track = s_frame;
        firsttrack = true;
        tracked = true;
    }

    //            if(!firsttrack){
    //               recogn->vesusmatch(mat_of_first,trackedRect,tmp_frame,foundRect,count_time);
    //            }


    //        if(trackedRect.size()!=0){
    //            ctracker.setMainImage(s_frame);
    //            ctracker.setCurrentRect(trackedRect[0]);
    //            Rect rect_track = ctracker.trackCurrentRect().boundingRect();
    //            if(rect_track.x>0 && rect_track.y >0 && rect_track.width*rect_track.height > 9){
    //                //                  if(!firsttrack){
    //                //                      recogn->vesusmatch(mat_track,trackedRect[0],s_frame,rect_track,count_time);
    //                //                  }
    //            }

    //            //               Rect2d rect_track = trackedRect[0];
    //            //               bool up = ptracker->update(s_frame,rect_track);
    //            //              if(up){
    //            //                  trackedRect.clear();
    //            //                  trackedRect[0] = rect_track;
    //            rectangle(s_frame,rect_track,cv::Scalar(255,255,255));
    //            //              }else{
    //            //                 trackedRect.clear();
    //            //              }
    //        }

    if(trackedtrace.width*trackedtrace.height!=0){
//        Ptr<Tracker> tracker = tl->getTracker();
        if(firsttrack){

            if((trackedtrace.x)+(trackedtrace.width)>s_frame.cols){
                trackedtrace.width = s_frame.cols - trackedtrace.x-1;
            }
            if((trackedtrace.y)+(trackedtrace.height)>s_frame.rows){
                trackedtrace.height = s_frame.rows - trackedtrace.y-1;
            }
            Rect2d rec = trackedtrace;
            ptracker->init(s_frame,rec);
            firsttrack = false;
        }else{
            if((trackedtrace.x)+(trackedtrace.width)>s_frame.cols){
                trackedtrace.width = s_frame.cols - trackedtrace.x-1;
            }
            if((trackedtrace.y)+(trackedtrace.height)>s_frame.rows){
                trackedtrace.height = s_frame.rows - trackedtrace.y-1;
            }
            Rect2d rec = trackedtrace;
            ptracker->update(s_frame,rec);
            rectangle(s_frame,rec,Scalar(255,255,255));
        }
    }

    if(count_time > 0 && count_time > 15){
        trackedtrace = Rect(0,0,0,0);
        tracked = false;
    }

    firsttrack = false;

    //rectangle camshift tracking objects

    //imshow("foreground", bgmask);
    imshow("background",back_frame);
    imshow("video",s_frame);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug("av init");
    av_register_all();
    avformat_network_init();
    count_time = 0;

    AVFormatContext* context = avformat_alloc_context();
    int video_stream_index=-1;
    AVDictionary *avdic=NULL;
    av_dict_set(&avdic,"rtsp_transport","tcp",0);
    av_dict_set(&avdic,"stimeout","1000000",0);
    av_dict_set(&avdic,"max_delay","50000",0);
    av_dict_set(&avdic,"fflags","nobuffer",0);
    av_dict_set(&avdic,"allowed_media_types","video",0);
    context->flags=AVFMT_FLAG_NOBUFFER;
    //open rtsp
    if(avformat_open_input(&context,"rtsp://192.168.2.12/1",NULL,&avdic) != 0){
        qDebug("can not open rtsp ");
        avformat_free_context(context);
    }
    if(avformat_find_stream_info(context,NULL) < 0){
        qDebug("can not find rtsp info");
        avformat_free_context(context);
    }

    for(int i =0;i<context->nb_streams;i++){
        if(context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            video_stream_index = i;
        }
    }
    AVPacket *packet=new AVPacket();
    av_init_packet(packet);
    av_read_play(context);

    AVCodecID codec_id=AV_CODEC_ID_H264;
    AVCodec *pCodec;
    AVCodecContext *pCodecCtx= NULL;
    AVCodecParserContext *pCodecParserCtx=NULL;
    AVFrame *pFrame;

    pCodec = avcodec_find_decoder(codec_id);
    if (!pCodec) {
        qDebug("Codec not found");
        return -1;
    }
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (!pCodecCtx){
        qDebug("Could not allocate video codec context");
        return -1;
    }

    pCodecParserCtx=av_parser_init(codec_id);
    if (!pCodecParserCtx){
        qDebug("Could not allocate video parser context");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        qDebug("Could not open codec");
        return -1;
    }

    int ret,got_picture;
    bool waitkey=true;
    pFrame = avcodec_alloc_frame() ;
    cvNamedWindow("video", 1);
    int count_f = 0;
    while(av_read_frame(context,packet)>=0)
    {
        if(waitkey)
        {
            if(packet->flags==1)
                waitkey=false;
            else
                continue;
        }

        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0) {
            qDebug("Decode Error.");
            continue;
        }
        //            cv::Mat frame(720, 1280, CV_8UC1);
        //            for(int i=0;i<720;i++)
        //            {
        //                memcpy(frame.data+i*1280,pFrame->data[0]+i*pFrame->linesize[0],1280);
        //            }
        //            Mat ved(pFrame->height, pFrame->width, CV_8UC3, pFrame->data[0], pFrame->linesize[0]);
        cv::Mat frame(720*3/2, 1280, CV_8UC1);
        for(int i=0;i<720;i++)
        {
            memcpy(frame.data+i*1280,pFrame->data[0]+i*pFrame->linesize[0],1280);
        }
        for(int i=0;i<720/2;i++)
        {
            memcpy(frame.data+720*1280+i*1280/2,pFrame->data[1]+i*pFrame->linesize[1],1280/2);
        }
        for(int i=0;i<720/2;i++)
        {
            memcpy(frame.data+720*1280*5/4+i*1280/2,pFrame->data[2]+i*pFrame->linesize[2],1280/2);
        }


        cv::cvtColor(frame,frame,CV_YUV2BGR_I420);

        //            IplImage img = (IplImage)(frame);
        resize(frame,frame,Size(600,400));
        camshifttracker::setMainImage(frame);
        system("echo hello");
        recogn->setThrehold(5);
        count_f++;
        if(count_f % 10==0){
            QDateTime dateTime_1 = QDateTime::currentDateTime();

            track_it(frame.clone());

            QDateTime dateTime_2 = QDateTime::currentDateTime();

            qDebug() << dateTime_1.msecsTo(dateTime_2);

            count_f = 0;
        }

        //           cvShowImage("video",&img);
        //           imshow("video",frame);
        cvWaitKey(1);
        //            waitKey(500);
        //            qDebug("got video");
    }

    av_free_packet(packet);

    return a.exec();
}
