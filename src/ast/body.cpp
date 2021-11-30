#include "body.hpp"

#include <iostream>

void Body::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_decls_) p_decls_->UpdateDepth(depth + 1);
  if (p_stmts_) p_stmts_->UpdateDepth(depth + 1);
}

void Body::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_decls_) p_decls_->Print(os);
  if (p_stmts_) p_stmts_->Print(os);
}
