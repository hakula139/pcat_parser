#ifndef SRC_AST_EXPR_HPP_
#define SRC_AST_EXPR_HPP_

#include <iostream>
#include <string>
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

 private:
  const std::string name_ = "expression";
};

class Exprs : public Nodes {
 public:
  explicit Exprs(const yy::location& loc) : Nodes{loc} {}

 private:
  const std::string name_ = "expression list";
  std::vector<SPtr<Expr>> data_;
};

class NumberExpr : public Expr {
 public:
  explicit NumberExpr(const yy::location& loc, SPtr<Number> p_number)
      : Expr{loc}, p_number_{p_number} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "number expression";
  SPtr<Number> p_number_;
};

class LvalueExpr : public Expr {
 public:
  explicit LvalueExpr(const yy::location& loc, SPtr<Lvalue> p_lvalue)
      : Expr{loc}, p_lvalue_{p_lvalue} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "lvalue expression";
  SPtr<Lvalue> p_lvalue_;
};

class ParenExpr : public Expr {
 public:
  explicit ParenExpr(const yy::location& loc, SPtr<Expr> p_expr)
      : Expr{loc}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "parenthesis expression";
  SPtr<Expr> p_expr_;
};

class UnaryExpr : public Expr {
 public:
  explicit UnaryExpr(const yy::location& loc, SPtr<Op> p_op, SPtr<Expr> p_expr)
      : Expr{loc}, p_op_{p_op}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "unary expression";
  SPtr<Op> p_op_;
  SPtr<Expr> p_expr_;
};

class BinaryExpr : public Expr {
 public:
  explicit BinaryExpr(
      const yy::location& loc,
      SPtr<Expr> p_expr1,
      SPtr<Op> p_op,
      SPtr<Expr> p_expr2)
      : Expr{loc}, p_expr1_{p_expr1}, p_op_{p_op}, p_expr2_{p_expr2} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "binary expression";
  SPtr<Expr> p_expr1_;
  SPtr<Op> p_op_;
  SPtr<Expr> p_expr2_;
};

class ActualParams;

class ProcCallExpr : public Expr {
 public:
  explicit ProcCallExpr(
      const yy::location& loc,
      SPtr<Id> p_id,
      SPtr<ActualParams> p_actual_params)
      : Expr{loc}, p_id_{p_id}, p_actual_params_{p_actual_params} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "procedure call expression";
  SPtr<Id> p_id_;
  SPtr<ActualParams> p_actual_params_;
};

class AssignExpr : public Expr {
 public:
  explicit AssignExpr(const yy::location& loc, SPtr<Id> p_id, SPtr<Expr> p_expr)
      : Expr{loc}, p_id_{p_id}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "assign expression";
  SPtr<Id> p_id_;
  SPtr<Expr> p_expr_;
};

class AssignExprs : public Nodes {
 public:
  explicit AssignExprs(const yy::location& loc) : Nodes{loc} {}

 private:
  const std::string name_ = "assign expression list";
  std::vector<SPtr<AssignExpr>> data_;
};

class CompValues : public Node {
 public:
  explicit CompValues(const yy::location& loc, SPtr<AssignExprs> p_assign_exprs)
      : Node{loc}, p_assign_exprs_{p_assign_exprs} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 private:
  const std::string name_ = "component values";
  SPtr<AssignExprs> p_assign_exprs_;
};

class RecordConstrExpr : public Expr {
 public:
  explicit RecordConstrExpr(
      const yy::location& loc, SPtr<Id> p_id, SPtr<CompValues> p_comp_values)
      : Expr{loc}, p_id_{p_id}, p_comp_values_{p_comp_values} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "record construction expression";
  SPtr<Id> p_id_;
  SPtr<CompValues> p_comp_values_;
};

class ArrayExpr : public Expr {
 public:
  explicit ArrayExpr(
      const yy::location& loc, SPtr<Expr> p_value, SPtr<Expr> p_num = nullptr)
      : Expr{loc}, p_value_{p_value}, p_num_{p_num} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "array expression";
  SPtr<Expr> p_value_;
  SPtr<Expr> p_num_;
};

class ArrayExprs : public Nodes {
 public:
  explicit ArrayExprs(const yy::location& loc) : Nodes{loc} {}

 private:
  const std::string name_ = "array expression list";
  std::vector<SPtr<ArrayExpr>> data_;
};

class ArrayValues : public Node {
 public:
  explicit ArrayValues(const yy::location& loc, SPtr<ArrayExprs> p_array_exprs)
      : Node{loc}, p_array_exprs_{p_array_exprs} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 private:
  const std::string name_ = "array values";
  SPtr<ArrayExprs> p_array_exprs_;
};

class ArrayConstrExpr : public Expr {
 public:
  explicit ArrayConstrExpr(
      const yy::location& loc, SPtr<Id> p_id, SPtr<ArrayValues> p_array_values)
      : Expr{loc}, p_id_{p_id}, p_array_values_{p_array_values} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "array construction expression";
  SPtr<Id> p_id_;
  SPtr<ArrayValues> p_array_values_;
};

class WriteExpr : public Expr {
 public:
  using UnionPtr = std::variant<SPtr<String>, SPtr<Expr>>;

  explicit WriteExpr(const yy::location& loc, UnionPtr p_write_expr)
      : Expr{loc}, p_write_expr_{p_write_expr} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 private:
  const std::string name_ = "write expression";
  UnionPtr p_write_expr_;
};

class WriteExprs : public Nodes {
 public:
  explicit WriteExprs(const yy::location& loc) : Nodes{loc} {}

 private:
  const std::string name_ = "write expression list";
  std::vector<SPtr<WriteExpr>> data_;
};

#endif  // SRC_AST_EXPR_HPP_
