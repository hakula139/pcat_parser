#ifndef SRC_AST_NODE_HPP_
#define SRC_AST_NODE_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"

class Node {
 public:
  using Ptr = std::unique_ptr<Node>;

  explicit Node(const yy::location& loc) : loc_{loc} {}

  virtual void UpdateDepth(int depth);
  virtual void Print(std::ostream& os) const;

  void set_loc(const yy::location& loc) { loc_ = loc; }
  void set_depth(int depth) { depth_ = depth; }

 protected:
  void PrintIndent(std::ostream& os) const;
  void PrintLocation(std::ostream& os) const;
  void PrintBase(std::ostream& os) const;

  const std::string name_ = "node";
  yy::location loc_;
  int depth_ = 0;
};

class Nodes : public Node {
 public:
  using Ptr = std::unique_ptr<Nodes>;

  explicit Nodes(const yy::location& loc) : Node{loc} {}

  void Insert(Node::Ptr node) { data_.push_back(node); }
  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  void operator+=(Node::Ptr node) { Insert(std::move(node)); }

 protected:
  const std::string name_ = "nodes";
  std::vector<Node::Ptr> data_;
};

#endif  // SRC_AST_NODE_HPP_
