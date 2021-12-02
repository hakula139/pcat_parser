#ifndef SRC_AST_PARAM_HPP_
#define SRC_AST_PARAM_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "expr.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "type.hpp"

class Param : public Node {
 public:
  explicit Param(const yy::location& loc) : Node{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "parameter";
};

class Params : public Nodes {
 public:
  explicit Params(const yy::location& loc) : Nodes{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "parameter list";
};

class FormalParam : public Param {
 public:
  explicit FormalParam(
      const yy::location& loc, SPtr<Ids> p_ids, SPtr<Type> p_type)
      : Param{loc}, p_ids_{p_ids}, p_type_{p_type} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "formal parameter";
  SPtr<Ids> p_ids_;
  SPtr<Type> p_type_;
};

class FormalParams : public Params {
 public:
  explicit FormalParams(const yy::location& loc) : Params{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "formal parameter list";
};

class ActualParams : public Exprs {
 public:
  explicit ActualParams(const yy::location& loc) : Exprs{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "actual parameter list";
};

class ReadParams : public Lvalues {
 public:
  explicit ReadParams(const yy::location& loc) : Lvalues{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "read parameter list";
};

class WriteParams : public WriteExprs {
 public:
  explicit WriteParams(const yy::location& loc) : WriteExprs{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "write parameter list";
};

#endif  // SRC_AST_PARAM_HPP_
