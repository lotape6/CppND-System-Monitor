#include "../include/file_reader.h"
#include "../include/linux_parser.h"
#include "gtest/gtest.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

const std::string dummyPrefix{"/home/lotape6/Projects/NanodegreeC++/CppND-System-Monitor/test/dummy_files"};

class utLinuxParser : public ::testing::Test
{
  public:
    // Got from :https://cplusplus.com/doc/tutorial/files/
    void writeFile(std::string file_path, std::vector<std::string> line_to_write)
    {
        std::ofstream foofile;
        foofile.open(file_path);
        for (auto line : line_to_write)
        {
            foofile << line << "\n";
        }
        foofile.close();
    }

  protected:
    FilesDictionary test_from;
    LinuxParser lp;
};

TEST_F(utLinuxParser, testReadNewEntry)
{
    const std::string dummyProc{dummyPrefix + kProcDirectory + kStatFilename};
    int user, nice, system, irq, softirq, steal, foo;
    std::string cpu_name;

    long cpu_usage0, cpu_usage0_copy, cpu_usage1;

    test_from[dummyProc].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal).update();
    cpu_usage0 = user + nice + system + irq + softirq + steal;

    test_from[dummyProc].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal);
    cpu_usage0_copy = user + nice + system + irq + softirq + steal;

    test_from[dummyProc].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal);
    cpu_usage1 = user + nice + system + irq + softirq + steal;

    EXPECT_EQ(cpu_usage0, cpu_usage0_copy);
    EXPECT_NE(cpu_usage0, cpu_usage1);
}

TEST_F(utLinuxParser, testReadWhileFileChanges)
{
    const std::string dummyProc{dummyPrefix + kProcDirectory + kStatFilename};
    std::vector<std::string> foo_content1{"DummyFile1 100", "Test 1"};
    std::vector<std::string> foo_content2{"DummyFile2 200", "Test 2"};

    std::string foostr1, foostr2;
    int fooint1, fooint2;

    const std::string test_file{dummyPrefix + "/testReadWhileFileChanges.txt"};

    writeFile(test_file, foo_content1);
    test_from[test_file].ReadLine(foostr1, fooint1);

    writeFile(test_file, foo_content2);
    test_from[test_file].update().ReadLine(foostr2, fooint2);

    EXPECT_NE(foostr1, foostr2);
    EXPECT_NE(fooint1, fooint2);
}

TEST_F(utLinuxParser, testReadUser)
{
    const std::string dummyProc{dummyPrefix + kProcDirectory + kStatFilename};

    std::string lotape = lp.UserFromId("1000");

    EXPECT_EQ(lotape, "lotape6");
}

TEST_F(utLinuxParser, DISABLED_testUid)
{
    const std::string dummyProc{dummyPrefix + kProcDirectory + kStatFilename};

    std::string lotape = lp.Uid(5657); // Harcoded PID :)

    EXPECT_EQ(lotape, "1000");
}

TEST_F(utLinuxParser, testCpuUtilization)
{
    std::string cpu_id;
    int user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;

    const std::string dummyProcStat{dummyPrefix + kProcDirectory + kStatFilename};
    test_from[dummyProcStat].ReadLines(0);
    test_from[dummyProcStat].ReadLine(cpu_id, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice);

    int prev_idle = 290665 + 1076; // 200 less than actual idle
    int actual_idle = idle + iowait;

    int prev_non_idle = 13566 + 583 + 5308 + 26; // 300 less than actual non idle
    int acutal_non_idle = user + nice + system + irq + softirq + steal;

    const int total = actual_idle + acutal_non_idle;
    const int total_diff = total - (prev_idle + prev_non_idle);
    const int idle_diff = actual_idle - prev_idle;

    float utilization = static_cast<float>(total_diff - idle_diff) / static_cast<float>(total_diff);
    EXPECT_TRUE(std::fabs(0.6 - utilization) < 0.01);
}