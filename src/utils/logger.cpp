#include "logger.hpp"

#include <iostream>
#include <string>

#include "../base/config.hpp"

inline void Logger::Log(const std::string& msg, std::ostream& output) {
  output << msg << "\n";
}

inline void Logger::Debug(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > DEBUG) return;
  output << "[DEBUG] ";
  Log(msg, output);
}

inline void Logger::Info(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > INFO) return;
  output << "[INFO ] ";
  Log(msg, output);
}

inline void Logger::Warn(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > WARN) return;
  output << "[WARN ] ";
  Log(msg, output);
}

inline void Logger::Error(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > ERROR) return;
  output << "[ERROR] ";
  Log(msg, output);
}

inline void Logger::Fatal(const std::string& msg, std::ostream& output) {
  if (LOG_LEVEL > FATAL) return;
  output << "[FATAL] ";
  Log(msg, output);
}
