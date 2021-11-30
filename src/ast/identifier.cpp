#include "identifier.hpp"

#include <iostream>

void Id::UpdateDepth(int depth) { Node::UpdateDepth(depth); }

void Id::Print(std::ostream& os) const {
  PrintIndent(os);
  os << name_ << " " << value_ << " ";
  PrintLocation(os);
}
