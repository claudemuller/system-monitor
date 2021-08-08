#include "system.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

Processor& System::Cpu() {
  return cpu_;
}

// TODO: Return a container composed of the system's processes
std::vector<Process>& System::Processes() {
  for (int pid : LinuxParser::Pids()) {
    processes_.emplace_back(
        Process(
            pid,
            LinuxParser::User(LinuxParser::Uid(pid)),
            LinuxParser::Command(pid),
            0.0, //LinuxParser::CpuUtilization(),//pid),
            LinuxParser::Ram(pid),
            LinuxParser::UpTime(pid))
        );
  }
  std::reverse(processes_.begin(), processes_.end());

  return processes_;
}

std::string System::Kernel() {
  kernel_ = LinuxParser::Kernel();
  return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() {
  os_name_ = LinuxParser::OperatingSystem();
  return os_name_;
}

int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

long int System::UpTime() {
  uptime_ = LinuxParser::UpTime();
  return uptime_;
}