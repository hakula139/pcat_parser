#include "logger.hpp"

#include <iostream>
#include <string>

#include "../base/config.hpp"
#include "../location.hpp"

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define RESET "\e[0;0m"

inline void Logger::Log(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (loc != location_type{}) output << loc << ": ";
  output << msg << "\n";
}

inline void Logger::Debug(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (LOG_LEVEL > DEBUG) return;
  output << GREEN << "[DEBUG] ";
  Log(msg, loc, output);
  output << RESET;
}

inline void Logger::Info(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (LOG_LEVEL > INFO) return;
  output << BLUE << "[INFO ] ";
  Log(msg, loc, output);
  output << RESET;
}

inline void Logger::Warn(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (LOG_LEVEL > WARN) return;
  output << YELLOW << "[WARN ] ";
  Log(msg, loc, output);
  output << RESET;
}

inline void Logger::Error(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (LOG_LEVEL > ERROR) return;
  output << RED << "[ERROR] ";
  Log(msg, loc, output);
  output << RESET;
}

inline void Logger::Fatal(
    const std::string& msg,
    const location_type& loc = {},
    std::ostream& output) {
  if (LOG_LEVEL > FATAL) return;
  output << PURPLE << "[FATAL] ";
  Log(msg, loc, output);
  output << RESET;
}
