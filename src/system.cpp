#include "system.h"

#include <linux_parser.h>

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

Processor& System::Cpu() { return cpu_; }

std::vector<Process>& System::Processes() {
  processes_ = {};

  for (int pid : LinuxParser::Pids()) {
    processes_.emplace_back(pid);
  }

  std::sort(processes_.begin(), processes_.end(),
            [](Process& a, Process& b) { return b < a; });

  return processes_;
}

std::string System::Kernel() {
  kernel_ = LinuxParser::Kernel();
  return kernel_;
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {
  os_name_ = LinuxParser::OperatingSystem();
  return os_name_;
}

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long int System::UpTime() { return LinuxParser::UpTime(); }