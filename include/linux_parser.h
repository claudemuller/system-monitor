#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// Parse strings
const std::string kMemTotalString{"MemTotal:"};
const std::string kMemFreeString{"MemFree:"};
const std::string kProcessString{"processes"};
const std::string kProcsRunningString{"procs_running"};
const std::string kVMDataString{"VmData:"};
const std::string kUidString{"Uid:"};
const std::string kOSNameString{"NAME"};
const std::string kOSVersionString{"NAME"};
const std::string kCPUString{"cpu"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();
std::vector<std::vector<long>> GetCPUStatInfo();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(std::string uid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif