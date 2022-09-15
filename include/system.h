#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

class System
{
  public:
    System(int n_proc = 0);
    Processor &Cpu(uint8_t n = 0); // max(n) == nproc + 1 (average[0] + each processor[1:n])
    std::vector<Process> &Processes();
    float MemoryUtilization();
    long UpTime();
    int TotalProcesses();
    int RunningProcesses();
    std::string Kernel();
    std::string OperatingSystem();

  private:
    std::vector<Processor> cpus_;
    std::vector<Process> processes_ = {};

    LinuxParser parser_;
};

#endif