#include <assert.h>
#include <dirent.h>
#include <filesystem>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Maig recursive variadic function got from: https://stackoverflow.com/a/26080768
// clang-format off
template <typename T>
void MagicRead(std::istringstream& i, T &t)
{
    i >> t;
}

template<typename T, typename... Args>
void MagicRead(std::istringstream& i, T &t, Args &&... args) // recursive variadic function
{
    MagicRead(i, t);
    MagicRead(i, args...);
}

template<typename... Args>
void ReadLine(std::ifstream &file, Args &&... args)
{
    string line;
    std::getline(file, line);
    std::istringstream linestream(line);
    MagicRead(linestream, args...);
}
// clang-format on

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    if (filestream.is_open())
    {
        while (std::getline(filestream, line))
        {
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), '=', ' ');
            std::replace(line.begin(), line.end(), '"', ' ');
            std::istringstream linestream(line);
            while (linestream >> key >> value)
            {
                if (key == "PRETTY_NAME")
                {
                    std::replace(value.begin(), value.end(), '_', ' ');
                    return value;
                }
            }
        }
    }
    return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
    if (KERNEL.empty())
    {
        string os, version;
        std::ifstream stream(kProcDirectory + kVersionFilename);
        if (stream.is_open())
        {
            ReadLine(stream, os, version, KERNEL);
        }
    }
    return KERNEL;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
    vector<int> pids;
    DIR *directory = opendir(kProcDirectory.c_str());
    struct dirent *file;
    while ((file = readdir(directory)) != nullptr)
    {
        // Is this a directory?
        if (file->d_type == DT_DIR)
        {
            // Is every character of the name a digit?
            string filename(file->d_name);
            if (std::all_of(filename.begin(), filename.end(), isdigit))
            {
                int pid = stoi(filename);
                pids.push_back(pid);
            }
        }
    }
    closedir(directory);
    return pids;
}

// Using the implementation of the king of kings: https://stackoverflow.com/a/41251290
float LinuxParser::MemoryUtilization()
{
    string tag, unit;
    int num;
    int mem_free, mem_total;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open())
    {

        ReadLine(stream, tag, num, unit);
        assert(tag.compare("MemTotal:") == 0);
        mem_total = num;

        ReadLine(stream, tag, num, unit);
        assert(tag.compare("MemFree:") == 0);
        mem_free = num;
    }
    else
    {
        assert(false);
    }

    return (mem_total - mem_free) / 0x1000000;
}

long LinuxParser::UpTime()
{
    long secs_up, secs_idle;

    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open())
    {
        ReadLine(stream, secs_up, secs_idle);
    }
    return secs_up;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies()
{
    return UpTime() * Hz();
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{

    return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
    return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
    return 0;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
    return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid [[maybe_unused]])
{
    return string();
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid [[maybe_unused]])
{
    return string();
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid [[maybe_unused]])
{
    return string();
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid [[maybe_unused]])
{
    return string();
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]])
{
    return 0;
}

int LinuxParser::Hz()
{
    std::ifstream stream(kBootDir + kConfig + Kernel());
    string line, id;

    while (getline(stream, line))
    {
        if (line.find("CONFIG_HZ=") != std::string::npos)
        {
            std::replace(line.begin(), line.end(), '=', ' ');
            std::istringstream(line) >> id >> HZ;
        }
    }
}