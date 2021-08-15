#ifndef PROCESS_H
#define PROCESS_H

#include <string>

const std::string kLineAbbr{"..."};

/**
 * Basic class for Process representation
 * It contains relevant attributes as shown below
 */
class Process {
 public:
  Process(int pid);

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_{0};
  std::string user_;
  std::string command_;
};

#endif