#include "stmt.hpp"

#include <iostream>

void AssignStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_lvalue_->UpdateDepth(depth + 1);
  p_expr_->UpdateDepth(depth + 1);
}

void AssignStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_lvalue_->Print(os);
  p_expr_->Print(os);
}

void ProcCallStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_actual_params_->UpdateDepth(depth + 1);
}

void ProcCallStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_id_->Print(os);
  p_actual_params_->Print(os);
}

void ReadStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_read_params_->UpdateDepth(depth + 1);
}

void ReadStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_read_params_->Print(os);
}

void WriteStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_write_params_->UpdateDepth(depth + 1);
}

void WriteStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_write_params_->Print(os);
}

void IfStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
  p_stmts_->UpdateDepth(depth + 1);
  p_elif_sections_->UpdateDepth(depth + 1);
  p_else_section_->UpdateDepth(depth + 1);
}

void IfStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_expr_->Print(os);
  p_stmts_->Print(os);
  p_elif_sections_->Print(os);
  p_else_section_->Print(os);
}

void ElifSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
  p_stmts_->UpdateDepth(depth + 1);
}

void ElifSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
  p_stmts_->UpdateDepth(depth + 1);
}

void ElseSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_stmts_->UpdateDepth(depth + 1);
}

void ElseSection::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_stmts_->UpdateDepth(depth + 1);
}

void WhileStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_expr_->Print(os);
  p_stmts_->Print(os);
}

void LoopStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_stmts_->UpdateDepth(depth + 1);
}

void LoopStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_stmts_->Print(os);
}

void ForStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_begin_->UpdateDepth(depth + 1);
  p_end_->UpdateDepth(depth + 1);
  p_step_->UpdateDepth(depth + 1);
  p_stmts_->UpdateDepth(depth + 1);
}

void ForStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_id_->Print(os);
  p_begin_->Print(os);
  p_end_->Print(os);
  p_step_->Print(os);
  p_stmts_->Print(os);
}

void ForStep::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
}

void ForStep::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
}

void ReturnStmt::UpdateDepth(int depth) {
  Stmt::UpdateDepth(depth);
  p_expr_->UpdateDepth(depth + 1);
}

void ReturnStmt::Print(std::ostream& os) const {
  Stmt::Print(os);
  p_expr_->Print(os);
}
