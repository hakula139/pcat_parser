#include "lvalue.hpp"

#include <iostream>
#include <string>

void IdLvalue::UpdateDepth(int depth) {
  Lvalue::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
}

std::string IdLvalue::value() const {
  auto id = p_id_ ? p_id_->value() : "";
  return id;
}

void ArrayElemLvalue::UpdateDepth(int depth) {
  Lvalue::UpdateDepth(depth);
  if (p_lvalue_) p_lvalue_->UpdateDepth(depth + 1);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

std::string ArrayElemLvalue::value() const {
  auto lvalue = p_lvalue_ ? p_lvalue_->value() : "";
  auto expr = p_expr_ ? p_expr_->value() : "";
  return lvalue + "[" + expr + "]";
}

void RecordCompLvalue::UpdateDepth(int depth) {
  Lvalue::UpdateDepth(depth);
  if (p_lvalue_) p_lvalue_->UpdateDepth(depth + 1);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
}

std::string RecordCompLvalue::value() const {
  auto lvalue = p_lvalue_ ? p_lvalue_->value() : "";
  auto id = p_id_ ? p_id_->value() : "";
  return lvalue + "." + id;
}
