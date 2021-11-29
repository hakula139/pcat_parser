#ifndef SRC_AST_PARAM_HPP_
#define SRC_AST_PARAM_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "node.hpp"

using ParamPtr = std::unique_ptr<Param>;
using ParamsPtr = std::unique_ptr<Params>;

class Param : public Node {
 public:
  explicit Param(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "parameter";
};

class Params : public Nodes {
 public:
  explicit Params(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "parameters";
};

class FormalParam : public Param {
 public:
  explicit FormalParam(const yy::location& loc, IdsPtr p_ids, TypePtr p_type)
      : Param{loc}, p_ids_{std::move(p_ids)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "formal parameter";
  IdsPtr p_ids_;
  TypePtr p_type_;
};

class FormalParams : public Params {
 public:
  explicit FormalParams(const yy::location& loc) : Params{loc} {}

 protected:
  const std::string name_ = "formal parameter list";
};

#endif  // SRC_AST_PARAM_HPP_
