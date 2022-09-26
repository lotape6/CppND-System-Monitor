#include <cmath>
#include <string>

#include "format.h"

using std::string;

string Format::GetFormattedTime(long secs)
{
    string time;

    int hours = static_cast<int>(floor(secs / 3600.));
    secs -= hours * 3600;

    int mins = static_cast<int>(floor(secs / 60.));
    secs -= mins * 60;

    string sec_prefix = secs < 10 ? "0" : "";
    string min_prefix = mins < 10 ? "0" : "";
    string hour_prefix = hours < 10 ? "0" : "";

    time = hour_prefix + std::to_string(hours) + ":" + min_prefix + std::to_string(mins) + ":" + sec_prefix +
           std::to_string(static_cast<int>(secs));

    return time;
}
string Format::ElapsedTime(long seconds)
{
    return GetFormattedTime(seconds);
}