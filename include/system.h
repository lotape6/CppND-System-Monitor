#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System
{
  public:
    System(){};
    Processor &Cpu(uint8_t n = 0); // max(n) == nproc + 1 (each proc[1:n] + average[0])
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
};

#endif