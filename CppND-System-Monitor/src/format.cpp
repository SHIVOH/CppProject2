#include <iostream>
#include <iomanip>
#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long time) {

    long hrs, mins, secs;
    hrs = time/3600;
    time = time%3600;
    mins = time/60;
    time = time%60;
    secs = time;
    

    std::ostringstream stream;
    stream << std::setw(2) << std::setfill('0') << hrs << ":" 
     << std::setw(2) << std::setfill('0') << mins << ":"
     << std::setw(2) << std::setfill('0') << secs;
    return stream.str();
}