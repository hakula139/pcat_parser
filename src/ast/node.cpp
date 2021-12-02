#include "node.hpp"

#include <iostream>
#include <string>

void Node::UpdateDepth(int depth) { set_depth(depth); }

void Node::Print(std::ostream& os) const {
  PrintBase(os);
  os << "\n";
}

void Node::PrintIndent(std::ostream& os) const {
  os << std::string(depth_ * 2, ' ');
}

void Node::PrintLocation(std::ostream& os) const {
  os << "@ <" << loc_.begin.line << ":" << loc_.begin.column << "-"
     << loc_.end.line << ":" << loc_.end.column << ">";
}

void Node::PrintBase(std::ostream& os) const {
  PrintIndent(os);
  os << name() << " ";
  PrintLocation(os);
}

void ValueNode::Print(std::ostream& os) const {
  PrintBase(os);
  os << " " << value() << "\n";
}

void Nodes::Insert(SPtr<Node> p_node) {
  set_loc(loc() + p_node->loc());
  data_.push_back(p_node);
}

void Nodes::InsertArray(SPtr<Nodes> p_nodes) {
  for (auto&& p_node : p_nodes->data_) {
    Insert(p_node);
  }
}

void Nodes::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  for (auto&& p_node : data_) {
    if (p_node) p_node->UpdateDepth(depth + 1);
  }
}

void Nodes::Print(std::ostream& os) const {
  Node::Print(os);
  for (const auto& p_node : data_) {
    if (p_node) p_node->Print(os);
  }
}
