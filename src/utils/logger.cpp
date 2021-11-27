#include "logger.hpp"

#include <iostream>
#include <string>

#include "../base/config.hpp"
#include "../parser.hpp"

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define RESET "\e[0;0m"

inline void Logger::Log(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (p_loc) output << *p_loc << ": ";
  output << msg << RESET << "\n";
}

inline void Logger::Debug(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (LOG_LEVEL <= DEBUG) {
    output << GREEN << "[DEBUG] ";
    Log(msg, p_loc, output);
  }
}

inline void Logger::Info(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (LOG_LEVEL <= INFO) {
    output << BLUE << "[INFO ] ";
    Log(msg, p_loc, output);
  }
}

inline void Logger::Warn(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (LOG_LEVEL <= WARN) {
    output << YELLOW << "[WARN ] ";
    Log(msg, p_loc, output);
  }
}

inline void Logger::Error(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (LOG_LEVEL <= ERROR) {
    output << RED << "[ERROR] ";
    Log(msg, p_loc, output);
  }
}

inline void Logger::Fatal(
    const std::string& msg,
    const yy::Parser::location_type* p_loc,
    std::ostream& output) {
  if (LOG_LEVEL <= FATAL) {
    output << PURPLE << "[FATAL] ";
    Log(msg, p_loc, output);
  }
}
