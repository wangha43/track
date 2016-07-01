#ifndef TLDTRACKER_H
#define TLDTRACKER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "opencv2/tracking.hpp"
#include "opencv2/tracking/tracker.hpp"
using namespace cv;

class tldtracker
{
public:
    tldtracker();
    Ptr<Tracker> getTracker();
private:
    Ptr<Tracker> tracker;
};

#endif // TLDTRACKER_H
