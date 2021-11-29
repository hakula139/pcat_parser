#include "body.hpp"

#include <iostream>

void Body::UpdateDepth(int depth) {
  set_depth(depth);
  p_decls_->set_depth(depth + 1);
  p_stmts_->set_depth(depth + 1);
}

void Body::Print(std::ostream& os) const {
  Node::Print(os);
  p_decls_->Print(os);
  p_stmts_->Print(os);
}
