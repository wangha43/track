#include "tldtracker.hpp"

tldtracker::tldtracker()
{
    tracker = Tracker::create("TLD");
}

Ptr<Tracker> tldtracker::getTracker(){
    return tracker;
}

