#ifndef SRC_AST_IDENTIFIER_HPP_
#define SRC_AST_IDENTIFIER_HPP_

#include <string>
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "node.hpp"

class Id : public ValueNode {
 public:
  explicit Id(const yy::location& loc, const std::string& value)
      : ValueNode{loc, value} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "identifier";
};

class Ids : public Nodes {
 public:
  explicit Ids(const yy::location& loc) : Nodes{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "identifier list";
  std::vector<SPtr<Id>> data_;
};

#endif  // SRC_AST_IDENTIFIER_HPP_
