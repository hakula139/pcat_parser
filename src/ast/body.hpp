#ifndef SRC_AST_BODY_HPP_
#define SRC_AST_BODY_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "decl.hpp"
#include "node.hpp"
#include "stmt.hpp"

using BodyPtr = std::unique_ptr<Body>;

class Body : public Node {
 public:
  explicit Body(const yy::location& loc, DeclsPtr p_decls, StmtsPtr p_stmts)
      : Node{loc}, p_decls_{std::move(p_decls)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "body";
  DeclsPtr p_decls_;
  StmtsPtr p_stmts_;
};

#endif  // SRC_AST_BODY_HPP_
