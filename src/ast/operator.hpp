#ifndef SRC_AST_OPERATOR_HPP_
#define SRC_AST_OPERATOR_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "node.hpp"

class Op;

using OpPtr = std::unique_ptr<Op>;

class Op : public Terminal<std::string> {
 public:
  explicit Op(const yy::location& loc, const std::string& value)
      : Terminal{loc, value} {}

 protected:
  const std::string name_ = "operator";
};

#endif  // SRC_AST_OPERATOR_HPP_
