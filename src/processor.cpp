#include "processor.h"

Processor::Processor(int processor_number)
    : processor_number_(processor_number), user_(raw_data_[0]), nice_(raw_data_[1]), system_(raw_data_[2]),
      idle_(raw_data_[3]), iowait_(raw_data_[4]), irq_(raw_data_[5]), softirq_(raw_data_[6]), steal_(raw_data_[7]),
      guest_(raw_data_[8]), guest_nice_(raw_data_[9])
{
    id_ = "Processor " + processor_number_ == 0 ? "Average" : std::to_string(processor_number_);
}

// Calculation based on: https://stackoverflow.com/a/23376195
float Processor::Utilization()
{
    prev_idle_ = actual_idle_;
    actual_idle_ = idle_ + iowait_;

    prev_non_idle_ = acutal_non_idle_;
    acutal_non_idle_ = user_ + nice_ + system_ + irq_ + softirq_ + steal_;

    const int total = actual_idle_ + acutal_non_idle_;
    const int total_diff = total - (prev_idle_ + prev_non_idle_);
    const int idle_diff = actual_idle_ - prev_idle_;

    return static_cast<float>(total_diff - idle_diff) / static_cast<float>(total_diff);
}

void Processor::UpdateRawData(int user, int nice, int system, int idle, int iowait, int irq, int softirq, int steal,
                              int guest, int guest_nice)
{
    std::swap(prev_raw_data_, raw_data_);

    raw_data_[0] = user;
    raw_data_[1] = nice;
    raw_data_[2] = system;
    raw_data_[3] = idle;
    raw_data_[4] = iowait;
    raw_data_[5] = irq;
    raw_data_[6] = softirq;
    raw_data_[7] = steal;
    raw_data_[8] = guest;
    raw_data_[9] = guest_nice;
}