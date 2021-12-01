#ifndef SRC_AST_OPERATOR_HPP_
#define SRC_AST_OPERATOR_HPP_

#include <memory>  // std::unique_ptr
#include <string>

#include "../location.hpp"
#include "expr.hpp"

class Op : public Expr {
 public:
  using Ptr = std::unique_ptr<Op>;

  explicit Op(const yy::location& loc, const std::string& value)
      : Expr{loc, value} {}

 protected:
  const std::string name_ = "operator";
};

#endif  // SRC_AST_OPERATOR_HPP_
