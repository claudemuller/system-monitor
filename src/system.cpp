#include "system.h"

#include <linux_parser.h>
#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

// TODO: Return the system's CPU
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

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  kernel_ = LinuxParser::Kernel();
  return kernel_;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// DONE: Return the operating system name
std::string System::OperatingSystem() {
  os_name_ = LinuxParser::OperatingSystem();
  return os_name_;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}

// DONE: Return the total number of processes on the system
int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}

// DONE: Return the number of seconds since the system started running
long int System::UpTime() {
  uptime_ = LinuxParser::UpTime();
  return uptime_;
}