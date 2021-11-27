#ifndef SRC_UTILS_LOGGER_HPP_
#define SRC_UTILS_LOGGER_HPP_

#include <iostream>
#include <string>

class Logger {
 public:
  static void Debug(const std::string& msg, std::ostream& output = std::cout);
  static void Info(const std::string& msg, std::ostream& output = std::cout);
  static void Warn(const std::string& msg, std::ostream& output = std::cout);
  static void Error(const std::string& msg, std::ostream& output = std::cout);

 protected:
  static void Log(const std::string& msg, std::ostream& output = std::cout);
};

#endif  // SRC_UTILS_LOGGER_HPP_
