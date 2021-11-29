#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <memory>   // std::unique_ptr
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "node.hpp"

using StmtPtr = std::unique_ptr<Stmt>;
using StmtsPtr = std::unique_ptr<Stmts>;

class Stmt : public Node {
 public:
  explicit Stmt(const yy::location& loc) : Node{loc} {}
};

class Stmts : public Node {
 public:
  explicit Stmts(const yy::location& loc) : Node{loc} {}

  void Insert(StmtPtr stmt) { data_.push_back(stmt); }
  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  void operator+=(StmtPtr stmt) { Insert(std::move(stmt)); }

 protected:
  std::vector<StmtPtr> data_;
};

#endif  // SRC_AST_STMT_HPP_
