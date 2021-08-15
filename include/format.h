#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string ElapsedTime(long times);
bool IsDigits(const std::string &str);
};  // namespace Format

#endif