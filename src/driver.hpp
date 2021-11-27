#ifndef SRC_DRIVER_HPP_
#define SRC_DRIVER_HPP_

#include <map>
#include <memory>  // std::unique_ptr
#include <string>

#include "base/config.hpp"
#include "parser.hpp"

// Define the signature of yylex() to return a symbol_type instead of int.
// NOLINTNEXTLINE(runtime/references)
#define YY_DECL yy::Parser::symbol_type yylex(Driver& drv)
YY_DECL;

class Body;

class Driver {
 public:
  int Parse(const std::string& input, const std::string& output);
  void ScanBegin();
  void ScanEnd();

  yy::location& loc() { return loc_; }

 private:
  bool trace_parsing_ = LOG_LEVEL <= DEBUG;
  bool trace_scanning_ = LOG_LEVEL <= DEBUG;

  std::unique_ptr<Body> p_program_;
  yy::location loc_;
};

#endif  // SRC_DRIVER_HPP_
