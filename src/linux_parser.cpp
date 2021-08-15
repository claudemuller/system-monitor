#include "linux_parser.h"

#include <format.h>
#include <unistd.h>

#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

template <typename T>
T getValueByProp(std::string const& prop, std::string const& filename);
template <typename T>
T getValueFromFile(std::string const& filename);

std::string LinuxParser::OperatingSystem() {
  std::string line, key, value, os_name;
  std::ifstream file_stream(kOSPath);

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + kOSPath +
                             "' for reading.");

  while (std::getline(file_stream, line)) {
    std::replace(line.begin(), line.end(), ' ', '_');
    std::replace(line.begin(), line.end(), '=', ' ');
    std::replace(line.begin(), line.end(), '"', ' ');
    std::istringstream line_stream(line);
    while (line_stream >> key >> value) {
      if (key == "NAME") {
        std::replace(value.begin(), value.end(), '_', ' ');
        os_name = value;
      }
      if (key == "VERSION") {
        std::replace(value.begin(), value.end(), '_', ' ');
        os_name += " " + value;
        return os_name;
      }
    }
  }

  file_stream.close();

  return value;
}

std::string LinuxParser::Kernel() {
  std::string os, kernel, version;
  std::string line;
  std::string file_path = kProcDirectory + kVersionFilename;
  std::ifstream file_stream(file_path);

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + file_path +
                             "' for reading.");

  std::getline(file_stream, line);
  std::istringstream line_stream(line);
  line_stream >> os >> version >> kernel;

  file_stream.close();

  return kernel;
}

std::vector<int> LinuxParser::Pids() {
  std::vector<int> pids;

  for (const auto& file : std::filesystem::directory_iterator(kProcDirectory)) {
    std::string filename = file.path().filename();
    if (file.is_directory() && Format::IsDigits(filename)) {
      int pid = stoi(filename);
      pids.push_back(pid);
    }
  }

  return pids;
}

float LinuxParser::MemoryUtilization() {
  float mem_total, mem_free;

  try {
    mem_total = getValueByProp<float>("MemTotal:", kMeminfoFilename);
    mem_free = getValueByProp<float>("MemFree:", kMeminfoFilename);
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return 1 - (mem_free / mem_total);
}

long LinuxParser::UpTime() {
  long uptime;

  try {
    uptime = getValueFromFile<long>(kUptimeFilename);
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return uptime;
}

long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {
  std::string file_path = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream file_stream(file_path);
  std::string line, value;
  long total = 0;

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + file_path +
                             "' for reading.");

  getline(file_stream, line);
  std::replace(line.begin(), line.end(), ':', ' ');
  std::istringstream line_stream(line);

  for (int i = 0; i < 22; i++) {
    line_stream >> value;
    if (i == 13 || i == 14 || i == 15 || i == 16) total += stol(value);
  }

  file_stream.close();

  return total;
}

std::vector<std::vector<long>> LinuxParser::GetCPUStatInfo() {
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::string line, category;
  std::string file_path = kProcDirectory + kStatFilename;
  std::ifstream file_stream(file_path);
  std::vector<std::vector<long>> cpus;

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + file_path +
                             "' for reading.");

  while (getline(file_stream, line)) {
    std::istringstream line_stream(line);
    line_stream >> category;
    if (category.rfind("cpu", 0) == 0) {
      line_stream >> user >> nice >> system >> idle >> iowait >> irq >>
          softirq >> steal >> guest >> guest_nice;
      std::vector<long> cpu{user, nice,    system, idle,  iowait,
                            irq,  softirq, steal,  guest, guest_nice};
      cpus.push_back(cpu);
    }
  }

  file_stream.close();

  return cpus;
}

long LinuxParser::ActiveJiffies() {
  long total = 0;

  try {
    for (auto cpu : GetCPUStatInfo()) {
      // user + nice + system
      total += cpu[0] + cpu[1] + cpu[2];
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return total;
}

long LinuxParser::IdleJiffies() {
  long total = 0;

  try {
    for (auto cpu : GetCPUStatInfo()) {
      // idle + iowait
      total += cpu[3] + cpu[4];
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return total;
}

std::vector<std::string> LinuxParser::CpuUtilization() {
  std::vector<std::string> result;

  try {
    for (auto cpu : GetCPUStatInfo()) {
      // user + nice + system
      float active = cpu[0] + cpu[1] + cpu[2];
      // idle + iowait
      float idle = cpu[3] + cpu[4];
      float percentage = active / (active + idle);
      result.push_back(std::to_string(percentage));
    }
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return result;
}

int LinuxParser::TotalProcesses() {
  int num_procs = 0;

  try {
    num_procs = getValueByProp<int>("processes", kStatFilename);
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return num_procs;
}

int LinuxParser::RunningProcesses() {
  int num_procs = 0;

  try {
    num_procs = getValueByProp<int>("procs_running", kStatFilename);
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return num_procs;
}

std::string LinuxParser::Command(int pid) {
  std::string cmd{};

  try {
    cmd = getValueFromFile<std::string>(std::to_string(pid) + kCmdlineFilename);
    ;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return cmd;
}

std::string LinuxParser::Ram(int pid) {
  long mb_size = 0;

  try {
    long kb_size =
        getValueByProp<long>("VmData:", std::to_string(pid) + kStatusFilename);
    mb_size = kb_size / 1000;
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return std::to_string(mb_size);
}

std::string LinuxParser::Uid(int pid) {
  std::string uid;

  try {
    uid = getValueByProp<std::string>("Uid:",
                                      std::to_string(pid) + kStatusFilename);
  } catch (const std::exception& ex) {
    std::cerr << ex.what() << std::endl;
  }

  return uid;
}

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

long LinuxParser::UpTime(int pid) {
  std::string file_path = kProcDirectory + std::to_string(pid) + kStatFilename;
  std::ifstream file_stream(file_path);
  std::string line, value;

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + file_path +
                             "' for reading.");

  getline(file_stream, line);
  std::replace(line.begin(), line.end(), ':', ' ');
  std::istringstream line_stream(line);

  for (int i = 0; i < 22; i++) {
    line_stream >> value;
  }

  file_stream.close();

  return UpTime() * sysconf(_SC_CLK_TCK) - stol(value);
}

template <typename T>
T getValueByProp(std::string const& prop, std::string const& filename) {
  T value;
  std::string line, key;
  bool found = false;
  std::ifstream file_stream(LinuxParser::kProcDirectory + filename);

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + filename +
                             "' for reading.");

  while (getline(file_stream, line)) {
    std::istringstream line_stream(line);
    while (line_stream >> key >> value) {
      if (key == prop) {
        found = true;
        break;
      }
    }
    if (found) break;
  }

  file_stream.close();

  return value;
}

template <typename T>
T getValueFromFile(std::string const& filename) {
  T value;
  std::string line;
  std::ifstream file_stream(LinuxParser::kProcDirectory + filename);

  if (!file_stream.is_open())
    throw std::runtime_error("Could not open file '" + filename +
                             "' for reading.");

  getline(file_stream, line);
  std::istringstream line_stream(line);
  line_stream >> value;

  file_stream.close();

  return value;
}
