#ifndef SRC_DRIVER_HPP_
#define SRC_DRIVER_HPP_

#include <string>

#include "ast/program.hpp"
#include "base/common.hpp"
#include "base/config.hpp"
#include "lexer.hpp"
#include "parser.hpp"

class Driver {
 public:
  Driver();

  int Parse(const std::string& in_path, const std::string& out_path);

  yy::Parser::location_type& loc() { return loc_; }
  void set_program(SPtr<Program> p_program) { p_program_ = p_program; }

 private:
  yy::Lexer lexer_;
  yy::Parser parser_;

  yy::Parser::location_type loc_;
  SPtr<Program> p_program_;

  bool trace_parsing_ = LOG_LEVEL <= TRACE;
  bool trace_scanning_ = LOG_LEVEL <= TRACE;
};

#endif  // SRC_DRIVER_HPP_
