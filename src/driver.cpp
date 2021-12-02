#include "driver.hpp"

#include <fstream>
#include <memory>  // std::make_unique
#include <string>

#include "parser.hpp"

Driver::Driver() : lexer_{this}, parser_{&lexer_, this} {
  lexer_.set_debug(trace_scanning_);
  parser_.set_debug_level(trace_parsing_);
}

int Driver::Parse(const std::string& in_path, const std::string& out_path) {
  loc_.initialize(&in_path);
  auto p_ifs = std::make_unique<std::ifstream>(in_path);
  auto p_ofs = std::make_unique<std::ofstream>(out_path);
  lexer_.switch_streams(*p_ifs, *p_ofs);
  int res = parser_();
  return res;
}
