#include "logger.hpp"

#include <iostream>
#include <string>

inline void Logger::Log(const std::string& msg, std::ostream& output) {
  output << msg << "\n";
}

inline void Logger::Debug(const std::string& msg, std::ostream& output) {
  output << "[DEBUG] ";
  Log(msg, output);
}

inline void Logger::Info(const std::string& msg, std::ostream& output) {
  output << "[INFO ] ";
  Log(msg, output);
}

inline void Logger::Info(const std::string& msg, std::ostream& output) {
  output << "[WARN ] ";
  Log(msg, output);
}

inline void Logger::Info(const std::string& msg, std::ostream& output) {
  output << "[ERROR] ";
  Log(msg, output);
}
