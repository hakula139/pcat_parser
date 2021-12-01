#ifndef SRC_DRIVER_HPP_
#define SRC_DRIVER_HPP_

#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "ast/program.hpp"
#include "base/config.hpp"
#include "lexer.hpp"
#include "parser.hpp"

class Driver {
 public:
  Driver();

  int Parse(const std::string& in_path, const std::string& out_path);

  yy::Parser::location_type& loc() { return loc_; }
  void set_program(Program::Ptr p_program) {
    p_program_ = std::move(p_program);
  }

 protected:
  yy::Lexer lexer_;
  yy::Parser parser_;

  yy::Parser::location_type loc_;
  Program::Ptr p_program_;

  bool trace_parsing_ = LOG_LEVEL <= TRACE;
  bool trace_scanning_ = LOG_LEVEL <= TRACE;
};

#endif  // SRC_DRIVER_HPP_
