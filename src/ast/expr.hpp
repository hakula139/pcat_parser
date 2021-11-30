#ifndef SRC_AST_EXPR_HPP_
#define SRC_AST_EXPR_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <variant>  // std::variant
#include <vector>

#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "param.hpp"

class Expr;
class Exprs;
class ParenExpr;
class UnaryExpr;
class BinaryExpr;
class ProcCallExpr;
class RecordConstrExpr;
class ArrayConstrExpr;
class WriteExpr;
class WriteExprs;
class AssignExpr;
class AssignExprs;
class ArrayExpr;
class ArrayExprs;

using ExprPtr = std::unique_ptr<Expr>;
using ExprsPtr = std::unique_ptr<Exprs>;
using ParenExprPtr = std::unique_ptr<ParenExpr>;
using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
using BinaryExprPtr = std::unique_ptr<BinaryExpr>;
using ProcCallExprPtr = std::unique_ptr<ProcCallExpr>;
using RecordConstrExprPtr = std::unique_ptr<RecordConstrExpr>;
using ArrayConstrExprPtr = std::unique_ptr<ArrayConstrExpr>;
using WriteExprPtr = std::unique_ptr<WriteExpr>;
using WriteExprsPtr = std::unique_ptr<WriteExprs>;
using AssignExprPtr = std::unique_ptr<AssignExpr>;
using AssignExprsPtr = std::unique_ptr<AssignExprs>;
using ArrayExprPtr = std::unique_ptr<ArrayExpr>;
using ArrayExprsPtr = std::unique_ptr<ArrayExprs>;

class Expr : public Node {
 public:
  explicit Expr(const yy::location& loc) : Node{loc} {}

  void Print(std::ostream& os) const override;
  virtual std::string value() const { return value_; }

 protected:
  const std::string name_ = "expression";
  const std::string value_;
};

class Exprs : public Nodes {
 public:
  explicit Exprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "expression list";
  std::vector<ExprPtr> data_;
};

class ParenExpr : public Expr {
 public:
  explicit ParenExpr(const yy::location& loc, ExprPtr p_expr)
      : Expr{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "parenthesis expression";
  ExprPtr p_expr_;
};

class UnaryExpr : public Expr {
 public:
  explicit UnaryExpr(const yy::location& loc, OpPtr p_op, ExprPtr p_expr)
      : Expr{loc}, p_op_{std::move(p_op)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "unary expression";
  OpPtr p_op_;
  ExprPtr p_expr_;
};

class BinaryExpr : public Expr {
 public:
  explicit BinaryExpr(
      const yy::location& loc, ExprPtr p_expr1, OpPtr p_op, ExprPtr p_expr2)
      : Expr{loc},
        p_expr1_{std::move(p_expr1)},
        p_op_{std::move(p_op)},
        p_expr2_{std::move(p_expr2)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "binary expression";
  ExprPtr p_expr1_;
  OpPtr p_op_;
  ExprPtr p_expr2_;
};

class ProcCallExpr : public Expr {
 public:
  explicit ProcCallExpr(
      const yy::location& loc, IdPtr p_id, ActualParamsPtr p_actual_params)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "procedure call expression";
  IdPtr p_id_;
  ActualParamsPtr p_actual_params_;
};

class RecordConstrExpr : public Expr {
 public:
  explicit RecordConstrExpr(
      const yy::location& loc, IdPtr p_id, CompValuesPtr p_comp_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_comp_values_{std::move(p_comp_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record construction expression";
  IdPtr p_id_;
  CompValuesPtr p_comp_values_;
};

class ArrayConstrExpr : public Expr {
 public:
  explicit ArrayConstrExpr(
      const yy::location& loc, IdPtr p_id, ArrayValuesPtr p_array_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_array_values_{std::move(p_array_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array construction expression";
  IdPtr p_id_;
  ArrayValuesPtr p_array_values_;
};

class WriteExpr : public Expr {
  using UnionPtr = std::variant<StringPtr, ExprPtr>;

 public:
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
  std::vector<WriteExprPtr> data_;
};

class AssignExpr : public Expr {
 public:
  explicit AssignExpr(const yy::location& loc, IdPtr p_id, ExprPtr p_expr)
      : Expr{loc}, p_id_{std::move(p_id)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "assign expression";
  IdPtr p_id_;
  ExprPtr p_expr_;
};

class AssignExprs : public Nodes {
 public:
  explicit AssignExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "assign expression list";
  std::vector<AssignExprPtr> data_;
};

class ArrayExpr : public Expr {
 public:
  explicit ArrayExpr(
      const yy::location& loc, ExprPtr p_value, ExprPtr p_num = nullptr)
      : Expr{loc}, p_value_{std::move(p_value)}, p_num_{std::move(p_num)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array expression";
  ExprPtr p_value_;
  ExprPtr p_num_;
};

class ArrayExprs : public Nodes {
 public:
  explicit ArrayExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "array expression list";
  std::vector<ArrayExprPtr> data_;
};

#endif  // SRC_AST_EXPR_HPP_
