#ifndef SRC_AST_BODY_HPP_
#define SRC_AST_BODY_HPP_

#include <iostream>
#include <string>

#include "../base/common.hpp"
#include "../location.hpp"
#include "decl.hpp"
#include "node.hpp"
#include "stmt.hpp"

class Body : public Node {
 public:
  explicit Body(
      const yy::location& loc, SPtr<Decls> p_decls, SPtr<Stmts> p_stmts)
      : Node{loc}, p_decls_{p_decls}, p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "body";
  SPtr<Decls> p_decls_;
  SPtr<Stmts> p_stmts_;
};

#endif  // SRC_AST_BODY_HPP_
