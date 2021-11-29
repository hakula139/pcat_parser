#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "node.hpp"

using StmtPtr = std::unique_ptr<Stmt>;
using StmtsPtr = std::unique_ptr<Stmts>;

class Stmt : public Node {
 public:
  explicit Stmt(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "statement";
};

class Stmts : public Node {
 public:
  explicit Stmts(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "statement list";
  std::vector<StmtPtr> data_;
};

#endif  // SRC_AST_STMT_HPP_
