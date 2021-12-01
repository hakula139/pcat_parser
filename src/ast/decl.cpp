#include "decl.hpp"

#include <iostream>

void VarDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  if (p_ids_) p_ids_->UpdateDepth(depth + 1);
  if (p_type_annot_) p_type_annot_->UpdateDepth(depth + 1);
  if (p_expr_) p_expr_->UpdateDepth(depth + 1);
}

void VarDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  if (p_ids_) p_ids_->Print(os);
  if (p_type_annot_) p_type_annot_->Print(os);
  if (p_expr_) p_expr_->Print(os);
}

void TypeDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_type_) p_type_->UpdateDepth(depth + 1);
}

void TypeDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  if (p_id_) p_id_->Print(os);
  if (p_type_) p_type_->Print(os);
}

void ProcDecl::UpdateDepth(int depth) {
  Decl::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_formal_params_) p_formal_params_->UpdateDepth(depth + 1);
  if (p_type_annot_) p_type_annot_->UpdateDepth(depth + 1);
  if (p_body_) p_body_->UpdateDepth(depth + 1);
}

void ProcDecl::Print(std::ostream& os) const {
  Decl::Print(os);
  if (p_id_) p_id_->Print(os);
  if (p_formal_params_) p_formal_params_->Print(os);
  if (p_type_annot_) p_type_annot_->Print(os);
  if (p_body_) p_body_->Print(os);
}
