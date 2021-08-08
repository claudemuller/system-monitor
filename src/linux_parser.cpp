#include "linux_parser.h"

#include <dirent.h>
#include <format.h>
#include <unistd.h>

#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

std::string LinuxParser::OperatingSystem() {
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kOSPath);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }

  return value;
}

std::string LinuxParser::Kernel() {
  std::string os, kernel, version;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }

  return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;
  for (const auto & file : std::filesystem::directory_iterator(kProcDirectory)) {
    std::string filename = file.path().filename();
    if (file.is_directory() && Format::IsDigits(filename)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }

  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string label, value;
  std::string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  float mem_total, mem_free, mem_available, buffers, cached, s_reclaimable, sh_mem;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> label >> value;
      if (label == "MemTotal:") mem_total = stof(value);
      if (label == "MemFree:") mem_free = stof(value);
      if (label == "MemAvailable:") mem_available = stof(value);
      if (label == "Buffers:") buffers = stof(value);
      if (label == "Cached:") cached = stof(value);
      if (label == "SReclaimable:") s_reclaimable = stof(value);
      if (label == "Shmem:") {
        sh_mem = stof(value);
        break;
      }
    }
  }

  float mem_used = mem_total - mem_free;
  float buffer_mem = mem_used - (buffers + cached);
  float cached_mem = cached + s_reclaimable - sh_mem;
  
  return  (mem_used + buffer_mem + buffers + cached_mem) / mem_total * 100 / 1000;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::string uptime, core_idle_time;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> uptime >> core_idle_time;
  }

  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
std::vector<std::string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return static_cast<int>(LinuxParser::Pids().size());
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string label, value;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> label >> value;
      if (label == "procs_running") return stoi(value);
    }
  }

  return 0;
}

// TODO: Read and return the command associated with a process
std::string LinuxParser::Command(int pid) {
  std::string cmd;
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> cmd;
  }

  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Ram(int pid[[maybe_unused]]) {
  std::string cmd;
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream line_stream(line);
    line_stream >> cmd;
  }

  return cmd;
}

// TODO: Read and return the user ID associated with a process
std::string LinuxParser::Uid(int pid) {
  std::string label, value;
  std::string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      line_stream >> label >> value;
      if (label == "Uid:") return value;
    }
  }

  return value;
}

// TODO: Read and return the user associated with a process
std::string LinuxParser::User(std::string uid) {
  std::string user, _, u;
  std::string line;
  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream line_stream(line);
      std::getline(line_stream, user, ':');
      std::getline(line_stream, _, ':');
      std::getline(line_stream, u, ':');
      if (u == uid) return user;
    }
  }

  return user;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

//LinuxParser::ReadProcessData(int pid) {
//  std::string label, value;
//  std::string line;
//  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
//  if (stream.is_open()) {
//    while (std::getline(stream, line)) {
//    }
//  }
//  return std::string();
//}