#include <linux_parser.h>
#include <unistd.h>
#include <string>

#include "process.h"

Process::Process(int pid) : pid_(pid) {
  // Parse the process command.
  std::string cmd = LinuxParser::Command(pid_);
  command_ = cmd.size() <= 40 ? cmd : cmd.substr(0, 39) + "...";

  // Extract the user that owns the process.
  std::string uid = LinuxParser::Uid(pid_);
  user_ = LinuxParser::User(uid);
}

int Process::Pid() {
  return pid_;
}

float Process::CpuUtilization() const {
  long active_jiffies = LinuxParser::ActiveJiffies(pid_);
  long uptime = LinuxParser::UpTime(pid_);

  return static_cast<float>(active_jiffies) / static_cast<float>(uptime);
}

std::string Process::Command() {
  return command_;
}

std::string Process::Ram() {
  return LinuxParser::Ram(pid_);
}

std::string Process::User() {
  return user_;
}

long int Process::UpTime() {
  // Return uptime in secs.
  return LinuxParser::UpTime(pid_) / sysconf(_SC_CLK_TCK);
}

bool Process::operator<(Process const& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
