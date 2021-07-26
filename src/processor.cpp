#include "processor.h"
#include "./linux_parser.h"
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  std::string _, user, nice, system, idle_, iowait, irq, softirq, steal;
  std::string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> _ >> user >> nice >> system >> idle_ >> iowait >> irq >> softirq >> steal;
  }
  //PrevIdle = previdle + previowait
  float idle = stof(idle_) + stof(iowait);
  //PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
  float non_idle = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(softirq) + stof(steal);
  //PrevTotal = PrevIdle + PrevNonIdle
  float total = idle + non_idle;
//  # differentiate: actual value minus the previous one
//  totald = Total - PrevTotal
//  idled = Idle - PrevIdle
  float cpu_percentage = static_cast<float>(total - idle)/total;
  return cpu_percentage;
}