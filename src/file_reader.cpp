
#include <assert.h>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

#include "file_reader.h"

FileReader::FileReader(std::string file) : file_path_(file), file_stream_(file_path_)
{
}

// Reset fstream state found on:https://stackoverflow.com/a/7681612
FileReader &FileReader::update()
{
    file_stream_.clear();
    file_stream_.seekg(0, std::ios::beg);
    return *this;
}

std::ifstream &FileReader::operator()()
{
    return file_stream_;
}

bool FileReader::isReady()
{
    return file_stream_.is_open();
}

FilesDictionary::FilesDictionary()
{
}

FileReader &FilesDictionary::operator[](std::string file_path)
{
    auto pos = dict_.find(file_path);
    if (pos == dict_.end())
    {
        dict_.insert(std::pair<std::string, FileReader>(file_path, FileReader(file_path)));
    }
    return dict_.at(file_path);
}

void FilesDictionary::update()
{
    for (auto it = dict_.begin(); it != dict_.end(); it++)
    {
        it->second.update();
    }
}
