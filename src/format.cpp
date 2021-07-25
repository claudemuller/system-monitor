#include <string>
#include "format.h"

#define SEC_IN_MIN 60
#define MIN_IN_HOUR 60

// DONE: Complete this helper function
std::string Format::ElapsedTime(long seconds) {
  auto min = seconds / SEC_IN_MIN;
  auto sec = seconds % SEC_IN_MIN;
  auto hour = min / MIN_IN_HOUR;
  min = min % MIN_IN_HOUR;
  return std::to_string(hour) + ":" + std::to_string(min) + ":" + std::to_string(sec);
}

bool Format::IsDigits(const std::string &str) {
  return str.find_first_not_of("0123456789") == std::string::npos;
}