#include "logger.hpp"

#include <iostream>
#include <string>

#include "../base/common.hpp"
#include "../base/config.hpp"
#include "../location.hpp"

void Logger::Log(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (p_loc) {
    auto loc = *p_loc;
    os << loc.begin.line << ":" << loc.begin.column << "-" << loc.end.line
       << ":" << loc.end.column << ": ";
  }
  os << msg << RESET << "\n";
}

void Logger::Trace(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= TRACE) {
    os << CYAN << "[TRACE] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Debug(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= DEBUG) {
    os << GREEN << "[DEBUG] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Info(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= INFO) {
    os << BLUE << "[INFO ] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Warn(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= WARN) {
    os << YELLOW << "[WARN ] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Error(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= ERROR) {
    os << RED << "[ERROR] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Fatal(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= FATAL) {
    os << PURPLE << "[FATAL] ";
    Log(msg, p_loc, os);
  }
}
