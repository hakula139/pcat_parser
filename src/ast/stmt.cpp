#include "stmt.hpp"

#include <iostream>

void AssignStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_lvalue_) p_lvalue_->UpdateDepth(depth + 1);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

void AssignStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_lvalue_) p_lvalue_->Print(os);
  if (p_expr_) p_expr_->Print(os);
}

void ProcCallStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_actual_params_) p_actual_params_->UpdateDepth(depth + 1);
}

void ProcCallStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_id_) p_id_->Print(os);
  if (p_actual_params_) p_actual_params_->Print(os);
}

void ReadStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_read_params_) p_read_params_->UpdateDepth(depth + 1);
}

void ReadStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_read_params_) p_read_params_->Print(os);
}

void WriteStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_write_params_) p_write_params_->UpdateDepth(depth + 1);
}

void WriteStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_write_params_) p_write_params_->Print(os);
}

void IfStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
  if (p_elif_sections_) p_elif_sections_->UpdateDepth(depth + 1);
  if (p_else_section_) p_else_section_->UpdateDepth(depth + 1);
}

void IfStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_expr_) p_expr_->Print(os);
  if (p_stmts_) p_stmts_->Print(os);
  if (p_elif_sections_) p_elif_sections_->Print(os);
  if (p_else_section_) p_else_section_->Print(os);
}

void ElifSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
}

void ElifSection::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_expr_) p_expr_->Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}

void ElseSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
}

void ElseSection::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}

void WhileStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_expr_) p_expr_->Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}

void LoopStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
}

void LoopStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}

void ForStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_begin_) p_begin_->UpdateDepth(depth + 1);
  if (p_end_) p_end_->UpdateDepth(depth + 1);
  if (p_step_) p_step_->UpdateDepth(depth + 1);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
}

void ForStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_id_) p_id_->Print(os);
  if (p_begin_) p_begin_->Print(os);
  if (p_end_) p_end_->Print(os);
  if (p_step_) p_step_->Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}

void ForStep::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

void ForStep::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_expr_) p_expr_->Print(os);
}

void ReturnStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

void ReturnStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  if (p_expr_) p_expr_->Print(os);
}
