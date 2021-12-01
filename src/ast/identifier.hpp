#ifndef SRC_AST_IDENTIFIER_HPP_
#define SRC_AST_IDENTIFIER_HPP_

#include <memory>  // std::unique_ptr
#include <string>
#include <vector>

#include "../location.hpp"
#include "expr.hpp"

class Id : public Expr {
 public:
  using Ptr = std::unique_ptr<Id>;

  explicit Id(const yy::location& loc, const std::string& value)
      : Expr{loc, value} {}

 protected:
  const std::string name_ = "identifier";
};

class Ids : public Nodes {
 public:
  using Ptr = std::unique_ptr<Ids>;

  explicit Ids(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "identifier list";
  std::vector<Id::Ptr> data_;
};

#endif  // SRC_AST_IDENTIFIER_HPP_
