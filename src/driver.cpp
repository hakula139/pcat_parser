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
  loc_.initialize();
  p_ifs_ = std::make_unique<std::ifstream>(in_path);
  p_ofs_ = std::make_unique<std::ofstream>(out_path);
  lexer_.switch_streams(ifs(), ofs());

  ofs() << "# " << in_path << "\n\n";
  int res = parser_();
  ofs() << "\n";

  if (p_program_) {
    p_program_->UpdateDepth(0);
    p_program_->Print(ofs());
  }
  return res;
}
