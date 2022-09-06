#include "../include/file_reader.h"
#include "../include/linux_parser.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

const std::string dummyPrefix{"/home/lotape6/Projects/NanodegreeC++/CppND-System-Monitor/test/dummy_files"};

class utLinuxParser : public ::testing::Test
{

  protected:
    FilesDictionary test_from;
};

TEST_F(utLinuxParser, testReadNewEntry)
{
    const std::string dummyProc{dummyPrefix + kProcDirectory + kStatFilename};
    int user, nice, system, irq, softirq, steal, foo;
    std::string cpu_name;

    long cpu_usage0, cpu_usage1;

    test_from[dummyProc].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal).update();

    cpu_usage0 = user + nice + system + irq + softirq + steal;

    test_from[dummyProc].ReadLine(cpu_name, user, nice, system, foo, foo, irq, softirq, steal).update();

    cpu_usage1 = user + nice + system + irq + softirq + steal;

    EXPECT_EQ(cpu_usage0, cpu_usage1);
}