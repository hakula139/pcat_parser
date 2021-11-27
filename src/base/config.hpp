#ifndef SRC_BASE_CONFIG_HPP_
#define SRC_BASE_CONFIG_HPP_

enum LogLevel {
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

constexpr const char* OUTPUT_DIR = "output";
constexpr LogLevel LOG_LEVEL = DEBUG;

#endif  // SRC_BASE_CONFIG_HPP_
