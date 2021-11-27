#include "driver.hpp"

#include <string>

#include "parser.hpp"

int Driver::Parse(const std::string& input, const std::string& output) {
  loc_.initialize(&input);
  ScanBegin();
  yy::Parser parser{*this};
  parser.set_debug_level(trace_parsing_);
  int res = parser();
  ScanEnd();
  return res;
}
