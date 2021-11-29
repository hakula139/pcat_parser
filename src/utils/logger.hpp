#ifndef SRC_UTILS_LOGGER_HPP_
#define SRC_UTILS_LOGGER_HPP_

#include <iostream>
#include <string>

#include "../location.hpp"

class Logger {
 public:
  static void Trace(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

  static void Debug(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

  static void Info(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

  static void Warn(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

  static void Error(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

  static void Fatal(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);

 protected:
  static void Log(
      const std::string& msg,
      const yy::location* p_loc = nullptr,
      std::ostream& os = std::cout);
};

#endif  // SRC_UTILS_LOGGER_HPP_
