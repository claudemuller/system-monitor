#include "format.h"

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string Format::ElapsedTime(long s) {
  std::chrono::seconds seconds{s};
  std::chrono::hours hours =
      std::chrono::duration_cast<std::chrono::hours>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

  std::chrono::minutes minutes =
      std::chrono::duration_cast<std::chrono::minutes>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

  std::stringstream ss{};

  ss << std::setw(2) << std::setfill('0') << hours.count() << std::setw(1)
     << ":" << std::setw(2) << std::setfill('0') << minutes.count()
     << std::setw(1) << ":" << std::setw(2) << std::setfill('0')
     << seconds.count();

  return ss.str();
}

bool Format::IsDigits(const std::string &str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}