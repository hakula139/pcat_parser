#include "logger.hpp"

#include <iostream>
#include <string>

#include "../base/config.hpp"

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define RESET "\e[0;0m"

inline void Logger::Log(const std::string& msg, std::ostream& output) {
  output << msg << "\n";
}

inline void Logger::Debug(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > DEBUG) return;
  output << GREEN << "[DEBUG] ";
  Log(msg, output);
  output << RESET;
}

inline void Logger::Info(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > INFO) return;
  output << BLUE << "[INFO ] ";
  Log(msg, output);
  output << RESET;
}

inline void Logger::Warn(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > WARN) return;
  output << YELLOW << "[WARN ] ";
  Log(msg, output);
  output << RESET;
}

inline void Logger::Error(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > ERROR) return;
  output << RED << "[ERROR] ";
  Log(msg, output);
  output << RESET;
}

inline void Logger::Fatal(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > FATAL) return;
  output << PURPLE << "[FATAL] ";
  Log(msg, output);
  output << RESET;
}
