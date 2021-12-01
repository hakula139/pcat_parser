#ifndef SRC_AST_OPERATOR_HPP_
#define SRC_AST_OPERATOR_HPP_

#include <memory>  // std::unique_ptr
#include <string>

#include "../location.hpp"
#include "expr.hpp"

class Op;

using OpPtr = std::unique_ptr<Op>;

class Op : public Expr {
 public:
  explicit Op(const yy::location& loc, const std::string& value)
      : Expr{loc, value} {}

 protected:
  const std::string name_ = "operator";
};

#endif  // SRC_AST_OPERATOR_HPP_
