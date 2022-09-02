#include "../include/file_reader.h"
#include "../include/linux_parser.h"
#include "gtest/gtest.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

class utLinuxParser : public ::testing::Test
{
  private:
    FilesDictionary test_from;
};

TEST_F(utLinuxParser, testReadNewEntry)
{
}