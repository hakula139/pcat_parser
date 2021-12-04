#ifndef SRC_AST_NODE_HPP_
#define SRC_AST_NODE_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"

class Node {
 public:
  explicit Node(const yy::location& loc) : loc_{loc} {}
  virtual ~Node() {}

  virtual void UpdateDepth(int depth);
  virtual void Print(std::ostream& os) const;

  virtual std::string name() const { return name_; }
  yy::location loc() const { return loc_; }
  void set_loc(const yy::location& loc) { loc_ = loc; }
  void set_depth(int depth) { depth_ = depth; }

 protected:
  void PrintIndent(std::ostream& os) const;
  void PrintLocation(std::ostream& os) const;
  void PrintBase(std::ostream& os) const;

 private:
  const std::string name_ = "node";
  yy::location loc_;
  int depth_ = 0;
};

class ValueNode : public Node {
 public:
  explicit ValueNode(const yy::location& loc, const std::string& value = "")
      : Node{loc}, value_{value} {}

  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }
  virtual std::string value() const { return value_; }

 private:
  const std::string name_ = "node";
  const std::string value_;
};

class Nodes : public Node {
 public:
  explicit Nodes(const yy::location& loc) : Node{loc} {}

  void Insert(SPtr<Node> p_node);
  void InsertArray(SPtr<Nodes> p_nodes);
  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "nodes";
  std::vector<SPtr<Node>> data_;
};

#endif  // SRC_AST_NODE_HPP_
