#ifndef SRC_AST_LVALUE_HPP_
#define SRC_AST_LVALUE_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"

class Lvalue : public ValueNode {
 public:
  explicit Lvalue(const yy::location& loc) : ValueNode{loc} {}

 protected:
  const std::string name_ = "lvalue";
};

class Lvalues : public Nodes {
 public:
  explicit Lvalues(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "lvalue list";
  std::vector<SPtr<Lvalue>> data_;
};

class IdLvalue : public Lvalue {
 public:
  explicit IdLvalue(const yy::location& loc, SPtr<Id> p_id)
      : Lvalue{loc}, p_id_{p_id} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "identifier lvalue";
  SPtr<Id> p_id_;
};

class Expr;

class ArrayElemLvalue : public Lvalue {
 public:
  explicit ArrayElemLvalue(
      const yy::location& loc, SPtr<Lvalue> p_lvalue, SPtr<Expr> p_expr)
      : Lvalue{loc}, p_lvalue_{p_lvalue}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array element lvalue";
  SPtr<Lvalue> p_lvalue_;
  SPtr<Expr> p_expr_;
};

class RecordCompLvalue : public Lvalue {
 public:
  explicit RecordCompLvalue(
      const yy::location& loc, SPtr<Lvalue> p_lvalue, SPtr<Id> p_id)
      : Lvalue{loc}, p_lvalue_{p_lvalue}, p_id_{p_id} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record component lvalue";
  SPtr<Lvalue> p_lvalue_;
  SPtr<Id> p_id_;
};

#endif  // SRC_AST_LVALUE_HPP_
