#ifndef SRC_AST_BODY_HPP_
#define SRC_AST_BODY_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move

#include "../base/common.hpp"
#include "../location.hpp"
#include "decl.hpp"
#include "node.hpp"
#include "stmt.hpp"

class Body : public Node {
 public:
  explicit Body(
      const yy::location& loc, UPtr<Decls> p_decls, UPtr<Stmts> p_stmts)
      : Node{loc}, p_decls_{std::move(p_decls)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "body";
  UPtr<Decls> p_decls_;
  UPtr<Stmts> p_stmts_;
};

#endif  // SRC_AST_BODY_HPP_
