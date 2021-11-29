#ifndef SRC_BASE_COMMON_HPP_
#define SRC_BASE_COMMON_HPP_

#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define RESET "\e[0;0m"

enum LogLevel {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

#endif  // SRC_BASE_COMMON_HPP_
