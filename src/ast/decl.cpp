#include "decl.hpp"

#include <iostream>

void VarDecl::UpdateDepth(int depth) {
  set_depth(depth);
  p_ids_->set_depth(depth + 1);
  p_type_annot_->set_depth(depth + 1);
  p_expr_->set_depth(depth + 1);
}

void VarDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_ids_->Print(os);
  p_type_annot_->Print(os);
  p_expr_->Print(os);
}

void TypeDecl::UpdateDepth(int depth) {
  set_depth(depth);
  p_id_->set_depth(depth + 1);
  p_type_->set_depth(depth + 1);
}

void TypeDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_id_->Print(os);
  p_type_->Print(os);
}

void ProcDecl::UpdateDepth(int depth) {
  set_depth(depth);
  p_id_->set_depth(depth + 1);
  p_formal_params_->set_depth(depth + 1);
  p_type_annot_->set_depth(depth + 1);
  p_body_->set_depth(depth + 1);
}

void ProcDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_id_->Print(os);
  p_formal_params_->Print(os);
  p_type_annot_->Print(os);
  p_body_->Print(os);
}
