#include "program.hpp"

#include <iostream>

void Program::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_body_->UpdateDepth(depth + 1);
}

void Program::Print(std::ostream& os) const {
  PrintIndent(os);
  os << name_ << " ";
  PrintLocation(os);
  os << "\n";
  p_body_->Print(os);
}
