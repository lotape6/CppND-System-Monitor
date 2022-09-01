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
    Processor &Cpu(uint8_t n = 0);     // max(n) == nproc + 1 (each proc[1:n] + average[0])
    std::vector<Process> &Processes(); // TODO: See src/system.cpp
    float MemoryUtilization();         // TODO: See src/system.cpp
    long UpTime();                     // TODO: See src/system.cpp
    int TotalProcesses();              // TODO: See src/system.cpp
    int RunningProcesses();            // TODO: See src/system.cpp
    std::string Kernel();              // TODO: See src/system.cpp
    std::string OperatingSystem();     // TODO: See src/system.cpp

  private:
    std::vector<Processor> cpus_;
    std::vector<Process> processes_ = {};
};

#endif