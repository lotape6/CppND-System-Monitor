#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor
{
  public:
    Processor(int proccessor_number);

    inline const std::string Id() const
    {
        return id_;
    }

    float Utilization();

    void UpdateRawData(int, int, int, int, int, int, int, int, int, int);

  private:
    std::string id_;
    int processor_number_;

    int raw_data_[10];
    int prev_raw_data_[10];

    // The /proc/stat data maped to raw_data array
    int &user_;
    int &nice_;
    int &system_;
    int &idle_;
    int &iowait_;
    int &irq_;
    int &softirq_;
    int &steal_;
    int &guest_;
    int &guest_nice_;

    // Calculations cache
    int actual_idle_, acutal_non_idle_;
    int prev_idle_, prev_non_idle_;
};

#endif
