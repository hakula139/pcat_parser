#include "program.hpp"

#include <iostream>

void Program::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_body_) p_body_->UpdateDepth(depth + 1);
}

void Program::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_body_) p_body_->Print(os);
}
