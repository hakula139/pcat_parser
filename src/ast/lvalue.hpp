#ifndef SRC_AST_LVALUE_HPP_
#define SRC_AST_LVALUE_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "expr.hpp"

class Lvalue;
class Lvalues;
class IdLvalue;
class ArrayElemLvalue;
class RecordCompLvalue;

using LvaluePtr = std::unique_ptr<Lvalue>;
using LvaluesPtr = std::unique_ptr<Lvalues>;
using IdLvaluePtr = std::unique_ptr<IdLvalue>;
using ArrayElemLvaluePtr = std::unique_ptr<ArrayElemLvalue>;
using RecordCompLvaluePtr = std::unique_ptr<RecordCompLvalue>;

class Lvalue : public Expr {
 public:
  explicit Lvalue(const yy::location& loc) : Expr{loc} {}

 protected:
  const std::string name_ = "lvalue";
};

class Lvalues : public Nodes {
 public:
  explicit Lvalues(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "lvalue list";
  std::vector<LvaluePtr> data_;
};

class IdLvalue : public Lvalue {
 public:
  explicit IdLvalue(const yy::location& loc, IdPtr p_id)
      : Lvalue{loc}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "identifier lvalue";
  IdPtr p_id_;
};

class ArrayElemLvalue : public Lvalue {
 public:
  explicit ArrayElemLvalue(
      const yy::location& loc, LvaluePtr p_lvalue, ExprPtr p_expr)
      : Lvalue{loc},
        p_lvalue_{std::move(p_lvalue)},
        p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array element lvalue";
  LvaluePtr p_lvalue_;
  ExprPtr p_expr_;
};

class RecordCompLvalue : public Lvalue {
 public:
  explicit RecordCompLvalue(
      const yy::location& loc, LvaluePtr p_lvalue, IdPtr p_id)
      : Lvalue{loc}, p_lvalue_{std::move(p_lvalue)}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record component lvalue";
  LvaluePtr p_lvalue_;
  IdPtr p_id_;
};

#endif  // SRC_AST_LVALUE_HPP_
