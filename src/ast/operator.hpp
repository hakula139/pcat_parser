#ifndef SRC_AST_OPERATOR_HPP_
#define SRC_AST_OPERATOR_HPP_

#include <string>

#include "../base/common.hpp"
#include "../location.hpp"
#include "node.hpp"

class Op : public ValueNode {
 public:
  explicit Op(const yy::location& loc, const std::string& value)
      : ValueNode{loc, value} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "operator";
};

#endif  // SRC_AST_OPERATOR_HPP_
