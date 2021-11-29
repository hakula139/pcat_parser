#ifndef SRC_DRIVER_HPP_
#define SRC_DRIVER_HPP_

#include <memory>  // std::unique_ptr
#include <string>

#include "base/config.hpp"
#include "lexer.hpp"
#include "parser.hpp"

class Body;

class Driver {
 public:
  Driver();

  int Parse(const std::string& in_path, const std::string& out_path);

  yy::Parser::location_type& loc() { return loc_; }

 protected:
  yy::Lexer lexer_;
  yy::Parser parser_;

  yy::Parser::location_type loc_;

  bool trace_parsing_ = LOG_LEVEL <= TRACE;
  bool trace_scanning_ = LOG_LEVEL <= TRACE;
};

#endif  // SRC_DRIVER_HPP_
