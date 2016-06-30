#include "test.h"
#include <unistd.h>
Test::Test(QObject *parent) :
    QObject(parent)
{
    busy=false;
    connect(this,SIGNAL(gotFrame(unsigned char*)),this,SLOT(oneFrame(unsigned char*)),Qt::QueuedConnection);
    cvNamedWindow("test", 1);
}

void Test::oneFrame(unsigned char *data)
{
    cv::Mat frame(720, 1280, CV_8UC1,data);
    busy=true;
    IplImage img = (IplImage)(frame);
    cvShowImage("test",&img);
    usleep(500000);
    busy=false;
}
