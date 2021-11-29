#include "stmt.hpp"

#include <iostream>

void Stmts::UpdateDepth(int depth) {
  set_depth(depth);
  for (auto&& p_stmt : data_) {
    p_stmt->set_depth(depth + 1);
  }
}

void Stmts::Print(std::ostream& os) const {
  PrintIndent(os);
  os << "stmts" << loc_ << "\n";
  for (const auto& p_stmt : data_) {
    p_stmt->Print(os);
  }
}
