#include "expr.hpp"

#include <iostream>
#include <string>

#include "../base/common.hpp"

void Expr::Print(std::ostream& os) const {
  PrintIndent(os);
  os << name_ << " ";
  PrintLocation(os);
  os << " " << value() << "\n";
}

void ParenExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
}

std::string ParenExpr::value() const { return "(" + p_expr_->value() + ")"; }

void UnaryExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_op_->UpdateDepth(depth + 1);
  p_expr_->UpdateDepth(depth + 1);
}

std::string UnaryExpr::value() const {
  return p_op_->value() + p_expr_->value();
}

void BinaryExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_expr1_->UpdateDepth(depth + 1);
  p_op_->UpdateDepth(depth + 1);
  p_expr2_->UpdateDepth(depth + 1);
}

std::string BinaryExpr::value() const {
  return p_expr1_->value() + p_op_->value() + p_expr2_->value();
}

void ProcCallExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_actual_params_->UpdateDepth(depth + 1);
}

std::string ProcCallExpr::value() const { return p_id_->value() + "()"; }

void RecordConstrExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_comp_values_->UpdateDepth(depth + 1);
}

std::string RecordConstrExpr::value() const { return p_id_->value() + "{}"; }

void ArrayConstrExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_array_values_->UpdateDepth(depth + 1);
}

std::string ArrayConstrExpr::value() const { return p_id_->value() + "[<>]"; }

void WriteExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  std::visit(
      Overloaded{
          [](auto&& p) { p->UpdateDepth(depth + 1); },
      },
      p_write_expr_);
}

std::string WriteExpr::value() const {
  return std::visit(
      Overloaded{
          [](const auto& p) { return p->value(); },
      },
      p_write_expr_);
}

void AssignExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_expr_->UpdateDepth(depth + 1);
}

std::string AssignExpr::value() const {
  return p_id_->value() + " = " + p_expr_->value();
}

void ArrayExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  p_value_->UpdateDepth(depth + 1);
  p_num_->UpdateDepth(depth + 1);
}

std::string AssignExpr::value() const {
  return p_id_->value() + " = " + p_expr_->value();
}
