#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

Process::Process(int pid, std::string user, std::string command, float cpu_utilisation, std::string ram, long int uptime) :
  pid_(pid), user_(user), command_(command), cpu_initialisation_(cpu_utilisation), ram_(ram), uptime_(uptime) {

}

// TODO: Return this process's ID
int Process::Pid() {
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
std::string Process::Command() {
  return command_;
}

// TODO: Return this process's memory utilization
std::string Process::Ram() {
  return ram_;
}

// TODO: Return the user (name) that generated this process
std::string Process::User() {
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return pid_ < a.pid_;
}