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

void Logger::Log(
    const std::string& msg, const yy::location* p_loc, std::ostream& os) {
  if (p_loc) os << *p_loc << ": ";
  os << msg << RESET << "\n";
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
