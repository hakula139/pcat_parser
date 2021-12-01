#ifndef SRC_AST_EXPR_HPP_
#define SRC_AST_EXPR_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <variant>  // std::variant
#include <vector>

#include "../location.hpp"
#include "constant.hpp"
#include "identifier.hpp"
#include "lvalue.hpp"
#include "node.hpp"
#include "operator.hpp"
#include "param.hpp"

class Expr : public Node {
 public:
  using Ptr = std::unique_ptr<Expr>;

  explicit Expr(const yy::location& loc, const std::string& value = "")
      : Node{loc}, value_{value} {}

  void Print(std::ostream& os) const override;
  virtual std::string value() const { return value_; }

 protected:
  const std::string name_ = "expression";
  const std::string value_;
};

class Exprs : public Nodes {
 public:
  using Ptr = std::unique_ptr<Exprs>;

  explicit Exprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "expression list";
  std::vector<Expr::Ptr> data_;
};

class NumberExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<NumberExpr>;

  explicit NumberExpr(const yy::location& loc, Number::Ptr p_number)
      : Expr{loc}, p_number_{std::move(p_number)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "number expression";
  Number::Ptr p_number_;
};

class LvalueExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<LvalueExpr>;

  explicit LvalueExpr(const yy::location& loc, Lvalue::Ptr p_lvalue)
      : Expr{loc}, p_lvalue_{std::move(p_lvalue)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "lvalue expression";
  Lvalue::Ptr p_lvalue_;
};

class ParenExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<ParenExpr>;

  explicit ParenExpr(const yy::location& loc, Expr::Ptr p_expr)
      : Expr{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "parenthesis expression";
  Expr::Ptr p_expr_;
};

class UnaryExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<UnaryExpr>;

  explicit UnaryExpr(const yy::location& loc, Op::Ptr p_op, Expr::Ptr p_expr)
      : Expr{loc}, p_op_{std::move(p_op)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "unary expression";
  Op::Ptr p_op_;
  Expr::Ptr p_expr_;
};

class BinaryExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<BinaryExpr>;

  explicit BinaryExpr(
      const yy::location& loc,
      Expr::Ptr p_expr1,
      Op::Ptr p_op,
      Expr::Ptr p_expr2)
      : Expr{loc},
        p_expr1_{std::move(p_expr1)},
        p_op_{std::move(p_op)},
        p_expr2_{std::move(p_expr2)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "binary expression";
  Expr::Ptr p_expr1_;
  Op::Ptr p_op_;
  Expr::Ptr p_expr2_;
};

class ProcCallExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<ProcCallExpr>;

  explicit ProcCallExpr(
      const yy::location& loc, Id::Ptr p_id, ActualParams::Ptr p_actual_params)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "procedure call expression";
  Id::Ptr p_id_;
  ActualParams::Ptr p_actual_params_;
};

class RecordConstrExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<RecordConstrExpr>;

  explicit RecordConstrExpr(
      const yy::location& loc, Id::Ptr p_id, CompValues::Ptr p_comp_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_comp_values_{std::move(p_comp_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record construction expression";
  Id::Ptr p_id_;
  CompValues::Ptr p_comp_values_;
};

class CompValues : public AssignExprs {
 public:
  using Ptr = std::unique_ptr<CompValues>;

  explicit CompValues(const yy::location& loc) : AssignExprs{loc} {}

 protected:
  const std::string name_ = "component values";
};

class ArrayConstrExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<ArrayConstrExpr>;

  explicit ArrayConstrExpr(
      const yy::location& loc, Id::Ptr p_id, ArrayValues::Ptr p_array_values)
      : Expr{loc},
        p_id_{std::move(p_id)},
        p_array_values_{std::move(p_array_values)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array construction expression";
  Id::Ptr p_id_;
  ArrayValues::Ptr p_array_values_;
};

class ArrayValues : public ArrayExprs {
 public:
  using Ptr = std::unique_ptr<ArrayValues>;

  explicit ArrayValues(const yy::location& loc) : ArrayExprs{loc} {}

 protected:
  const std::string name_ = "array values";
};

class WriteExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<WriteExpr>;
  using UnionPtr = std::variant<String::Ptr, Expr::Ptr>;

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
  using Ptr = std::unique_ptr<WriteExprs>;

  explicit WriteExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "write expression list";
  std::vector<WriteExpr::Ptr> data_;
};

class AssignExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<AssignExpr>;

  explicit AssignExpr(const yy::location& loc, Id::Ptr p_id, Expr::Ptr p_expr)
      : Expr{loc}, p_id_{std::move(p_id)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "assign expression";
  Id::Ptr p_id_;
  Expr::Ptr p_expr_;
};

class AssignExprs : public Nodes {
 public:
  using Ptr = std::unique_ptr<AssignExprs>;

  explicit AssignExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "assign expression list";
  std::vector<AssignExpr::Ptr> data_;
};

class ArrayExpr : public Expr {
 public:
  using Ptr = std::unique_ptr<ArrayExpr>;

  explicit ArrayExpr(
      const yy::location& loc, Expr::Ptr p_value, Expr::Ptr p_num = nullptr)
      : Expr{loc}, p_value_{std::move(p_value)}, p_num_{std::move(p_num)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array expression";
  Expr::Ptr p_value_;
  Expr::Ptr p_num_;
};

class ArrayExprs : public Nodes {
 public:
  using Ptr = std::unique_ptr<ArrayExprs>;

  explicit ArrayExprs(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "array expression list";
  std::vector<ArrayExpr::Ptr> data_;
};

#endif  // SRC_AST_EXPR_HPP_
