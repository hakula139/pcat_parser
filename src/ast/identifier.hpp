#ifndef SRC_AST_IDENTIFIER_HPP_
#define SRC_AST_IDENTIFIER_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "node.hpp"

class Id;
class Ids;

using IdPtr = std::unique_ptr<Id>;
using IdsPtr = std::unique_ptr<Ids>;

class Id : public Terminal {
 public:
  explicit Id(const yy::location& loc, const std::string& value)
      : Terminal{loc, value} {}

 protected:
  const std::string name_ = "identifier";
};

class Ids : public Nodes {
 public:
  explicit Ids(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "identifier list";
  std::vector<IdPtr> data_;
};

#endif  // SRC_AST_IDENTIFIER_HPP_
