#include "expr.hpp"

#include <iostream>
#include <string>

#include "../base/common.hpp"

void NumberExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_number_) p_number_->UpdateDepth(depth + 1);
}

std::string NumberExpr::value() const {
  auto number = p_number_ ? p_number_->value() : "";
  return number;
}

void LvalueExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_lvalue_) p_lvalue_->UpdateDepth(depth + 1);
}

std::string LvalueExpr::value() const {
  auto lvalue = p_lvalue_ ? p_lvalue_->value() : "";
  return lvalue;
}

void ParenExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

std::string ParenExpr::value() const {
  auto expr = p_expr_ ? p_expr_->value() : "";
  return "(" + expr + ")";
}

void UnaryExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_op_) p_op_->UpdateDepth(depth + 1);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

std::string UnaryExpr::value() const {
  auto op = p_op_ ? p_op_->value() : "";
  auto expr = p_expr_ ? p_expr_->value() : "";
  return op + expr;
}

void BinaryExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_expr1_) p_expr1_->UpdateDepth(depth + 1);
  if (p_op_) p_op_->UpdateDepth(depth + 1);
  if (p_expr2_) p_expr2_->UpdateDepth(depth + 1);
}

std::string BinaryExpr::value() const {
  auto expr1 = p_expr1_ ? p_expr1_->value() : "";
  auto op = p_op_ ? p_op_->value() : "";
  auto expr2 = p_expr2_ ? p_expr1_->value() : "";
  return expr1 + " " + op + " " + expr2;
}

void ProcCallExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_actual_params_) p_actual_params_->UpdateDepth(depth + 1);
}

std::string ProcCallExpr::value() const {
  auto id = p_id_ ? p_id_->value() : "";
  return id + "()";
}

void RecordConstrExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_comp_values_) p_comp_values_->UpdateDepth(depth + 1);
}

std::string RecordConstrExpr::value() const {
  auto id = p_id_ ? p_id_->value() : "";
  return id + "{}";
}

void ArrayConstrExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_array_values_) p_array_values_->UpdateDepth(depth + 1);
}

std::string ArrayConstrExpr::value() const {
  auto id = p_id_ ? p_id_->value() : "";
  return id + "[<>]";
}

void WriteExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  auto visitor = Overloaded{
      [depth](auto&& p) {
        if (p) p->UpdateDepth(depth + 1);
      },
  };
  std::visit(visitor, p_write_expr_);
}

std::string WriteExpr::value() const {
  auto visitor = Overloaded{
      [](const auto& p) {
        auto value = p ? p->value() : "";
        return value;
      },
  };
  return std::visit(visitor, p_write_expr_);
}

void AssignExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

std::string AssignExpr::value() const {
  auto id = p_id_ ? p_id_->value() : "";
  auto expr = p_expr_ ? p_expr_->value() : "";
  return id + " = " + expr;
}

void ArrayExpr::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  if (p_value_) p_value_->UpdateDepth(depth + 1);
  if (p_num_) p_num_->UpdateDepth(depth + 1);
}

std::string ArrayExpr::value() const {
  auto value = p_value_ ? p_value_->value() : "";
  auto num = p_num_ ? p_num_->value() + " OF " : "";
  return num + value;
}
