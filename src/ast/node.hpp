#ifndef SRC_AST_NODE_HPP_
#define SRC_AST_NODE_HPP_

#include "../location.hpp"

class Node {
 public:
  explicit Node(const yy::location& loc) : loc_{loc} {}

  void set_loc(const yy::location& loc) { loc_ = loc; }

 protected:
  yy::location loc_;
};

#endif  // SRC_AST_NODE_HPP_
