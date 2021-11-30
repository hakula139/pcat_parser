#include "node.hpp"

#include <iostream>
#include <string>

void Node::UpdateDepth(int depth) { set_depth(depth); }

void Node::Print(std::ostream& os) const {
  PrintIndent(os);
  os << name_ << " ";
  PrintLocation(os);
  os << "\n";
}

void Node::PrintIndent(std::ostream& os) const {
  os << std::string(depth_ * 2, ' ');
}

void Node::PrintLocation(std::ostream& os) const { os << loc_; }

void Nodes::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  for (auto&& p_node : data_) {
    p_node->UpdateDepth(depth + 1);
  }
}

void Nodes::Print(std::ostream& os) const {
  Node::Print(os);
  for (const auto& p_node : data_) {
    p_node->Print(os);
  }
}
