#ifndef SRC_AST_EXPR_HPP_
#define SRC_AST_EXPR_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move
#include <variant>  // std::variant
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "constant.hpp"
#include "identifier.hpp"
#include "lvalue.hpp"
#include "node.hpp"
#include "operator.hpp"

class Expr : public ValueNode {
 public:
  explicit Expr(const yy::location& loc, const std::string& value = "")
      : ValueNode{loc, value} {}

 protected:
  const std::string name_ = "expression";
};

class Exprs : public Nodes {
 public:
  explicit Exprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "expression list";
  std::vector<UPtr<Expr>> data_;
};

class NumberExpr : public Expr {
 public:
  explicit NumberExpr(const yy::location& loc, UPtr<Number> p_number)
      : Expr{loc}, p_number_{std::move(p_number)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "number expression";
  UPtr<Number> p_number_;
};

class LvalueExpr : public Expr {
 public:
  explicit LvalueExpr(const yy::location& loc, UPtr<Lvalue> p_lvalue)
      : Expr{loc}, p_lvalue_{std::move(p_lvalue)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "lvalue expression";
  UPtr<Lvalue> p_lvalue_;
};

class ParenExpr : public Expr {
 public:
  explicit ParenExpr(const yy::location& loc, UPtr<Expr> p_expr)
      : Expr{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "parenthesis expression";
  UPtr<Expr> p_expr_;
};

class UnaryExpr : public Expr {
 public:
  explicit UnaryExpr(const yy::location& loc, UPtr<Op> p_op, UPtr<Expr> p_expr)
      : Expr{loc}, p_op_{std::move(p_op)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "unary expression";
  UPtr<Op> p_op_;
  UPtr<Expr> p_expr_;
};

class BinaryExpr : public Expr {
 public:
  explicit BinaryExpr(
      const yy::location& loc,
      UPtr<Expr> p_expr1,
      UPtr<Op> p_op,
      UPtr<Expr> p_expr2)
      : Expr{loc},
        p_expr1_{std::move(p_expr1)},
        p_op_{std::move(p_op)},
        p_expr2_{std::move(p_expr2)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "binary expression";
  UPtr<Expr> p_expr1_;
  UPtr<Op> p_op_;
  UPtr<Expr> p_expr2_;
};

class ActualParams;

class ProcCallExpr : public Expr {
 public:
  explicit ProcCallExpr(
      const yy::location& loc,
      UPtr<Id> p_id,
      UPtr<ActualParams> p_actual_params)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "procedure call expression";
  UPtr<Id> p_id_;
  UPtr<ActualParams> p_actual_params_;
};

class AssignExpr : public Expr {
 public:
  explicit AssignExpr(const yy::location& loc, UPtr<Id> p_id, UPtr<Expr> p_expr)
      : Expr{loc}, p_id_{std::move(p_id)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "assign expression";
  UPtr<Id> p_id_;
  UPtr<Expr> p_expr_;
};

class AssignExprs : public Nodes {
 public:
  explicit AssignExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "assign expression list";
  std::vector<UPtr<AssignExpr>> data_;
};

class CompValues : public AssignExprs {
 public:
  explicit CompValues(const yy::location& loc) : AssignExprs{loc} {}

 protected:
  const std::string name_ = "component values";
};

class RecordConstrExpr : public Expr {
 public:
  explicit RecordConstrExpr(
      const yy::location& loc, UPtr<Id> p_id, UPtr<CompValues> p_comp_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_comp_values_{std::move(p_comp_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record construction expression";
  UPtr<Id> p_id_;
  UPtr<CompValues> p_comp_values_;
};

class ArrayExpr : public Expr {
 public:
  explicit ArrayExpr(
      const yy::location& loc, UPtr<Expr> p_value, UPtr<Expr> p_num = nullptr)
      : Expr{loc}, p_value_{std::move(p_value)}, p_num_{std::move(p_num)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array expression";
  UPtr<Expr> p_value_;
  UPtr<Expr> p_num_;
};

class ArrayExprs : public Nodes {
 public:
  explicit ArrayExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "array expression list";
  std::vector<UPtr<ArrayExpr>> data_;
};

class ArrayValues : public ArrayExprs {
 public:
  explicit ArrayValues(const yy::location& loc) : ArrayExprs{loc} {}

 protected:
  const std::string name_ = "array values";
};

class ArrayConstrExpr : public Expr {
 public:
  explicit ArrayConstrExpr(
      const yy::location& loc, UPtr<Id> p_id, UPtr<ArrayValues> p_array_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_array_values_{std::move(p_array_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array construction expression";
  UPtr<Id> p_id_;
  UPtr<ArrayValues> p_array_values_;
};

class WriteExpr : public Expr {
 public:
  using UnionPtr = std::variant<UPtr<String>, UPtr<Expr>>;

  explicit WriteExpr(const yy::location& loc, UnionPtr p_write_expr)
      : Expr{loc}, p_write_expr_{std::move(p_write_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "write expression";
  UnionPtr p_write_expr_;
};

class WriteExprs : public Nodes {
 public:
  explicit WriteExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "write expression list";
  std::vector<UPtr<WriteExpr>> data_;
};

#endif  // SRC_AST_EXPR_HPP_
