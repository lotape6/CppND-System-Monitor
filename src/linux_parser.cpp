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
        std::string foo;
        from[kProcDirectory + kVersionFilename].ReadLine(foo, foo, KERNEL).update();
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

    from[kProcDirectory + kMeminfoFilename].ReadLine(tag, num, unit);
    assert(tag.compare("MemTotal:") == 0);
    mem_total = num;

    from[kProcDirectory + kMeminfoFilename].ReadLine(tag, num, unit).update();
    assert(tag.compare("MemFree:") == 0);
    mem_free = num;

    return (mem_total - mem_free) / static_cast<float>(0x1000000);
}

long LinuxParser::UpTime()
{
    long secs_up, secs_idle;

    from[kProcDirectory + kUptimeFilename].ReadLine(secs_up, secs_idle).update();

    return secs_up;
}

long LinuxParser::Jiffies()
{
    return UpTime() / TICK;
}

// Calculation based on: https://stackoverflow.com/a/23376195
long LinuxParser::ActiveJiffies(int pid)
{
    long user_jiffies, kernel_jiffies;
    string foo;

    from[kProcDirectory + "/" + std::to_string(pid) + kStatFilename]
        .ReadLine(foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, foo, user_jiffies, kernel_jiffies)
        .update();

    return user_jiffies + kernel_jiffies;
}

// Calculation based on: https://stackoverflow.com/a/23376195
long LinuxParser::ActiveJiffies()
{
    int user, nice, system, irq, softirq, steal, foo;
    string cpu_name;

    from[kProcDirectory + kStatFilename].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal).update();

    return user + nice + system + irq + softirq + steal;
}

// Calculation based on: https://stackoverflow.com/a/23376195
long LinuxParser::IdleJiffies()
{
    int idle, iowait, foo;
    string cpu_name;

    from[kProcDirectory + kStatFilename].ReadLine(cpu_name, foo, foo, foo, idle, iowait).update();

    return idle + iowait;
}

vector<string> LinuxParser::CpuUtilization(int n)
{
    std::string line, aux;
    std::vector<string> utilization;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open())
    {
        for (int i = 0; i < n; ++i)
        {
            std::getline(stream, line);
        }

        size_t index = string::npos;
        std::getline(stream, line);
        while (string::npos != (index = line.find(" ")))
        {
            utilization.push_back(line.substr(0, index));
            line.erase(0, index + 1); // 1 == " "
        }
    }

    return utilization;
}

int LinuxParser::TotalProcesses()
{
    return static_cast<int>(Pids().size());
}

int LinuxParser::RunningProcesses()
{
    int running_procs = 0;
    std::string foo, status, file_path;

    for (auto &pid : Pids())
    {
        file_path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
        from[file_path].update().ReadLines(2).ReadLine(foo, status, foo);
        running_procs += status == "R" ? 1 : 0;
    }
    return running_procs;
}

string LinuxParser::Command(int pid)
{
    std::string file_path, cmdline;
    file_path = kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename;
    from[file_path].update().ReadLine(cmdline);
    return cmdline;
}

// Got from https://unix.stackexchange.com/questions/224015/memory-usage-of-a-given-process-using-linux-proc-filesystem
string LinuxParser::Ram(int pid)
{
    std::string file_path, foo, ram, units;
    file_path = kProcDirectory + "/" + std::to_string(pid) + KSmapsFilename;
    from[file_path].update().ReadLines(1).ReadLine(foo, ram, units);

    return ram + " " + units;
}

string LinuxParser::Uid(int pid)
{
    std::string file_path, foo, uid;
    file_path = kProcDirectory + "/" + std::to_string(pid) + kStatusFilename;
    from[file_path].update().ReadLines(8).ReadLine(foo, uid);

    return uid;
}

string LinuxParser::User(int pid)
{
    return UserFromId(Uid(pid));
}

// String splitting got from: https://stackoverflow.com/a/10058725
string LinuxParser::UserFromId(std::string userid)
{
    std::string user, uid, foo;

    user = "";
    uid = "";
    auto &pass_file = from[kPasswordPath].update();
    do
    {
        pass_file.ReadLine(foo);
        std::stringstream line(foo);
        std::getline(line, user, ':');
        std::getline(line, foo, ':');
        std::getline(line, uid, ':');

    } while (uid != userid);

    return user;
}

long LinuxParser::UpTime(int pid)
{
    long jiffies = ActiveJiffies(pid);

    return jiffies / TICK;
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
    return HZ;
}