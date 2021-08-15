#include "processor.h"

#include "./linux_parser.h"

double Processor::Utilization() {
  prev_active_ = current_active_;
  prev_total_ = current_total_;
  current_active_ = LinuxParser::ActiveJiffies();
  current_total_ = LinuxParser::Jiffies();

  float total = current_total_ - prev_total_;
  float active = current_active_ - prev_active_;

  return active / total;
}