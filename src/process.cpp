#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : cached_cpu(-1.), pid_(pid)
{
}

int Process::Pid()
{
    return pid_;
}

// Got from suggested SO answer: https://stackoverflow.com/a/16736599
float Process::CpuUtilization()
{
    long activeJiffies = parser_.ActiveJiffies(pid_);
    long uptime = parser_.UpTime();
    float Hz = static_cast<float>(parser_.Hz());

    cached_cpu = static_cast<float>(activeJiffies / Hz) / uptime;
    return cached_cpu;
}

string Process::Command()
{
    return parser_.Command(pid_);
}

string Process::Ram()
{
    return parser_.Ram(pid_);
}

string Process::User()
{
    return parser_.User(pid_);
}

long int Process::UpTime()
{
    return parser_.UpTime() - parser_.UpTime(pid_);
}

bool Process::operator<(Process const &a) const
{
    if (cached_cpu != -1 && a.cached_cpu != -1)
    {
        return (cached_cpu < a.cached_cpu);
    }
    else
    {
        return false;
    }
}