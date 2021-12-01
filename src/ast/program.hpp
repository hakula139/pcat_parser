#ifndef SRC_AST_PROGRAM_HPP_
#define SRC_AST_PROGRAM_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move

#include "../base/common.hpp"
#include "../location.hpp"
#include "body.hpp"
#include "node.hpp"

class Program : public Node {
 public:
  explicit Program(const yy::location& loc, UPtr<Body> p_body)
      : Node{loc}, p_body_{std::move(p_body)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "program";
  UPtr<Body> p_body_;
};

#endif  // SRC_AST_PROGRAM_HPP_
