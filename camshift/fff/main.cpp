#include <QApplication>
#include <opencv2/opencv.hpp>
#include <QThread>
#include <iostream>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/core.hpp"
#include "opencv2/objdetect.hpp"
#include "detect.hpp"
#include "camshift.hpp"
#include "recognize.hpp"

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
 bool tracker_sys=false;
 bool moving = false;
Rect boundRect;
Mat mat_of_first;
vector<int> count_time;

detecter * dt = new detecter;
recognizer * recogn = new recognizer;

vector<Rect> trackedRect;
vector<Rect> foundRect;
bool firsttrack = true;
void track_it(Mat & s_frame){
    Mat kframe,bgmask,back_frame,tmp_frame;
    cvtColor(s_frame,tmp_frame,COLOR_BGR2GRAY);
    blur(tmp_frame,kframe,Size(3,3),Point(-1,-1));
    Ptr<BackgroundSubtractorMOG2> bgsubtractor = dt->bgsubtractor;
    bgsubtractor->apply(kframe, bgmask, (true) ? -1 : 0);
    bgsubtractor->getBackgroundImage(back_frame);

    //init the tracked Rect
    if(firsttrack){
        trackedRect = dt->findarea(bgmask,tmp_frame);
        if(trackedRect.size()>0){
            size_t siz = trackedRect.size();
            vector<int> k(siz,0);
            count_time.insert(count_time.end(),k.begin(),k.end());
        }
    }else{
        foundRect = dt->findarea(bgmask,tmp_frame);
    }

      if(!firsttrack){
         recogn->vesusmatch(mat_of_first,trackedRect,tmp_frame,foundRect,count_time);
      }
      mat_of_first = tmp_frame;
      firsttrack = false;

      for(size_t i = camtracker.size();i<trackedRect.size();i++){
//
            camshifttracker ctracker;
            ctracker.setCurrentRect(trackedRect[i]);
          camtracker.push_back(ctracker);
      }



      //rectangle camshift tracking objects
      rectang.clear();
for (size_t i=0; i<camtracker.size(); i++) {
    if(camtracker[i].trackCurrentRect().boundingRect().area() <=1)
            continue;
     cout<<camtracker[i].trackCurrentRect().boundingRect().height<<endl;
     rectang.push_back(camtracker[i].trackCurrentRect().boundingRect());
//    rectangle(frame,camtracker[i].trackCurrentRect().boundingRect(),cv::Scalar(255, 255, 255));
}
//      camtracker.clear();
//imshow("foreground", bgmask);
imshow("background",back_frame);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug("av init");
    av_register_all();
    avformat_network_init();

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
            count_f++;
            camshifttracker::setMainImage(frame);
            if(count_f % 5==0){
                track_it(frame);
                count_f = 0;
            }

            for(size_t i = 0;i<rectang.size();i++){
                rectangle(frame,rectang[i],cv::Scalar(255, 255, 255));
            }

//            cvShowImage("video",&img);
            imshow("video",frame);
            cvWaitKey(1);
//            waitKey(500);
//            qDebug("got video");
        }

        av_free_packet(packet);

    return a.exec();
}
