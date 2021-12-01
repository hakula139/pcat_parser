#ifndef SRC_AST_PROGRAM_HPP_
#define SRC_AST_PROGRAM_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "body.hpp"
#include "node.hpp"

class Program : public Node {
 public:
  using Ptr = std::unique_ptr<Program>;

  explicit Program(const yy::location& loc, Body::Ptr p_body)
      : Node{loc}, p_body_{std::move(p_body)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "program";
  Body::Ptr p_body_;
};

#endif  // SRC_AST_PROGRAM_HPP_
