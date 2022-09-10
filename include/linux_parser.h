#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include "file_reader.h"
#include <fstream>
#include <regex>
#include <string>
#include <unistd.h>

// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string KSmapsFilename{"/smaps"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};
const std::string kBootDir{"/boot"};
const std::string kConfig{"/config-"};

class LinuxParser
{
  public:
    const long TICK = sysconf(_SC_CLK_TCK);

    // System
    float MemoryUtilization();
    long UpTime();
    std::vector<int> Pids();
    int TotalProcesses();
    int RunningProcesses();
    std::string OperatingSystem();
    std::string Kernel();

    // CPU
    enum CPUStates
    {
        kUser_ = 0,
        kNice_,
        kSystem_,
        kIdle_,
        kIOwait_,
        kIRQ_,
        kSoftIRQ_,
        kSteal_,
        kGuest_,
        kGuestNice_
    };
    std::vector<std::string> CpuUtilization(int n = 0);
    long Jiffies();
    long ActiveJiffies();
    long ActiveJiffies(int pid);
    long IdleJiffies();
    int Hz();

    // Processes
    std::string Command(int pid);
    std::string Ram(int pid);
    std::string Uid(int pid);
    std::string User(int pid);
    std::string UserFromId(std::string userid);
    long int UpTime(int pid);

    std::string KERNEL;
    int HZ;

    FilesDictionary from;
}; // namespace LinuxParser

#endif