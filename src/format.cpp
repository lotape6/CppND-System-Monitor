#include <cmath>
#include <string>

#include "format.h"

using std::string;

string Format::GetFormattedTime(long secs)
{
    string time = "HH:MM:SS";

    int hours = static_cast<int>(floor(secs / 3600.));
    secs -= hours * 3600;

    int mins = static_cast<int>(floor(secs / 60.));
    secs -= mins * 60;

    time = std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(static_cast<int>(secs));

    return time;
}
string Format::ElapsedTime(long seconds [[maybe_unused]])
{
    return string();
}