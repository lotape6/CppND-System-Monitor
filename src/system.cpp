#include <unistd.h>

#include <assert.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System(int n_proc)
{
    ++n_proc; // +1 due to the average
    for (int i = 0; i < n_proc; i++)
    {
        cpus_.emplace_back(i);
    }

    std::vector<int> processes_pids = parser_.Pids();

    for (auto pid : processes_pids)
    {
        processes_.emplace_back(pid);
    }
}

Processor &System::Cpu(uint8_t n)
{
    assert(cpus_.size() > 0);
    return cpus_.at(n);
}

vector<Process> &System::Processes()
{
    return processes_;
}

std::string System::Kernel()
{
    return parser_.Kernel();
}

float System::MemoryUtilization()
{
    return parser_.MemoryUtilization();
}

std::string System::OperatingSystem()
{
    return parser_.OperatingSystem();
}

int System::RunningProcesses()
{
    return parser_.RunningProcesses();
}

int System::TotalProcesses()
{
    return parser_.TotalProcesses();
}

long int System::UpTime()
{
    return parser_.UpTime();
}
