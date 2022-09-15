#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid)
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
    float Hz = static_cast<float>(parser_.Hz());
    long uptime = parser_.UpTime();

    cached_cpu = 100 * (static_cast<float>(activeJiffies / Hz) / uptime);
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
    return parser_.UpTime(pid_);
}

bool Process::operator<(Process const &a) const
{
    return (cached_cpu < a.cached_cpu);
}