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
  os << msg;
  if (&os == &std::cout) os << RESET;
  os << "\n";
}

void Logger::Trace(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= TRACE) {
    if (&os == &std::cout) os << CYAN;
    os << "[TRACE] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Debug(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= DEBUG) {
    if (&os == &std::cout) os << GREEN;
    os << "[DEBUG] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Info(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= INFO) {
    if (&os == &std::cout) os << BLUE;
    os << "[INFO ] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Warn(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= WARN) {
    if (&os == &std::cout) os << YELLOW;
    os << "[WARN ] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Error(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= ERROR) {
    if (&os == &std::cout) os << RED;
    os << "[ERROR] ";
    Log(msg, p_loc, os);
  }
}

void Logger::Fatal(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (LOG_LEVEL <= FATAL) {
    if (&os == &std::cout) os << PURPLE;
    os << "[FATAL] ";
    Log(msg, p_loc, os);
  }
}
