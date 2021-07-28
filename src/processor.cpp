#include "processor.h"
#include "./linux_parser.h"
#include <string>

#include <iostream>

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

  prev_idle_ += prev_io_wait_;
  std::cout << prev_idle_ << std::endl;

  float idle = stof(idle_) + stof(io_wait);
  prev_non_idle_ = prev_user_ + prev_nice_ + prev_system_ + prev_irq_ + prev_soft_irq_ + prev_steal_;
  std::cout << prev_non_idle_ << " " << idle << std::endl;

  float non_idle = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(soft_irq) + stof(steal);
  prev_total_ = prev_idle_ + prev_non_idle_;
  std::cout << prev_total_ << " " << non_idle << std::endl;

  float total = idle + non_idle;

  float total_d = total - prev_total_;
  float idle_d = idle - prev_idle_;
  float cpu_percentage = static_cast<float>(total_d - idle_d)/total_d;

  std::cout << cpu_percentage << std::endl;

  return cpu_percentage;
}