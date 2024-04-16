#pragma once
#include <bits/stdc++.h>
#include "Utils/Tools_Fun.hpp"
 
class Clock
{
    time_t Now;
    time_t Last;
    time_t Origin;
    
    std::string printTime(time_t time);               // print time point
    std::string printDT(time_t time1, time_t time2);  // print time interval

public:
    Clock();
    void reset();                                     // reset to current time
    std::string exportNow();                          // output current time
    std::string exportTime();                         // output time interval after resetting
    std::string tick();                               // output time interval after last tick
    
};

Clock::Clock()
{
    reset();
}

void Clock::reset()
{
    Now = time(0);
    Last = Now;
    Origin = Now;
}

std::string Clock::printTime(time_t time)
{
    std::string Info = ctime(&time);
    
    // delete last term '\n' for exportNow()
    if (!Info.empty() && Info.back() == '\n') Info.pop_back();

    return Info;
}

std::string Clock::printDT(time_t time1, time_t time2)
{
    time_t dt = time2 - time1;
    int s = dt % 60;
    dt = dt / 60;
    int min = dt % 60;
    dt = dt / 60;
    int h = dt % 24;
    int day = (int)(dt / 24);
    std::string Info = toStr(day) + " day " + toStr(h) + ":" + toStr(min) + ":" + toStr(s);
    return Info;
}

std::string Clock::exportNow()
{
    time_t now = time(0);
    return printTime(now);
}

std::string Clock::exportTime()
{
    return printDT(Origin, time(0));
}

std::string Clock::tick()
{
    Last = Now;
    Now = time(0);
    return printDT(Last, Now);
}
