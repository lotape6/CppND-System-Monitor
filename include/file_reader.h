#ifndef FILE_READER_H
#define FILE_READER_H

#include <assert.h>
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <vector>

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
    std::string line;
    std::getline(file, line);
    std::istringstream linestream(line);
    MagicRead(linestream, args...);
}
// clang-format on

class FileReader
{
  public:
    FileReader(std::string file);

    // Reset fstream state found on:https://stackoverflow.com/a/7681612
    FileReader &update();

    std::ifstream &operator()();
    bool isReady();
    template <typename... Args> FileReader &ReadLine(Args &&... args)
    {
        if (isReady())
        {
            std::string line;
            std::getline(file_stream_, line);
            std::istringstream linestream(line);
            MagicRead(linestream, args...);
        }
        else
        {
            // assert(false);
        }
        return *this;
    }
    FileReader &ReadLines(int n_lines);

  private:
    std::string file_path_;
    std::ifstream file_stream_;
};

class FilesDictionary
{
  public:
    FilesDictionary();
    FileReader &operator[](std::string file_path);

    void update();

  private:
    std::unordered_map<std::string, FileReader> dict_;
};

#endif // FILE_READER_H