#include "tldtracker.hpp"

tldtracker::tldtracker()
{
    tracker = Tracker::create("KCF");
}

Ptr<Tracker> tldtracker::getTracker(){
    return tracker;
}

