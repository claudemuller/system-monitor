#include "processor.h"
#include "./linux_parser.h"
#include <string>
#include <cmath>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string _, user, nice, system, idle_, io_wait, irq, soft_irq, steal;
  std::string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> _ >> user >> nice >> system >> idle_ >> io_wait >> irq >> soft_irq >> steal;
  }

  float idle = stof(idle_) + stof(io_wait);
  float non_idle = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(soft_irq) + stof(steal);
  float total = idle + non_idle;

  prev_idle_ = prev_idle_ + prev_io_wait_;
  prev_non_idle_ = prev_user_ + prev_nice_ + prev_system_ + prev_irq_ + prev_soft_irq_ + prev_steal_;
  prev_total_ = prev_idle_ + prev_non_idle_;

  float total_d = total - prev_total_;
  float idle_d = idle - prev_idle_;

  prev_idle_ = idle;
  prev_io_wait_ = stof(io_wait);
  prev_user_ = stof(user);
  prev_nice_ = stof(nice);
  prev_system_ = stof(system);
  prev_irq_ = stof(irq);
  prev_soft_irq_ = stof(soft_irq);
  prev_steal_ = stof(steal);

  float cpu_percentage = fabs(static_cast<float>(total_d - idle_d)/total_d * 5);

  return cpu_percentage;
}