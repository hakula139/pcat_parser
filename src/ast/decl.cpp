#include "decl.hpp"

#include <iostream>

#include "body.hpp"

void VarDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  p_ids_->UpdateDepth(depth + 1);
  p_type_annot_->UpdateDepth(depth + 1);
  p_expr_->UpdateDepth(depth + 1);
}

void VarDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_ids_->Print(os);
  p_type_annot_->Print(os);
  p_expr_->Print(os);
}

void TypeDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_type_->UpdateDepth(depth + 1);
}

void TypeDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_id_->Print(os);
  p_type_->Print(os);
}

void ProcDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_formal_params_->UpdateDepth(depth + 1);
  p_type_annot_->UpdateDepth(depth + 1);
  p_body_->UpdateDepth(depth + 1);
}

void ProcDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  p_id_->Print(os);
  p_formal_params_->Print(os);
  p_type_annot_->Print(os);
  p_body_->Print(os);
}
