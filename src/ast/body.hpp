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

class Body : public Node {
 public:
  using Ptr = std::unique_ptr<Body>;

  explicit Body(const yy::location& loc, Decls::Ptr p_decls, Stmts::Ptr p_stmts)
      : Node{loc}, p_decls_{std::move(p_decls)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "body";
  Decls::Ptr p_decls_;
  Stmts::Ptr p_stmts_;
};

#endif  // SRC_AST_BODY_HPP_
