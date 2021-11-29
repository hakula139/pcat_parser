#include "type.hpp"

#include <iostream>

void TypeAnnot::UpdateDepth(int depth) {
  set_depth(depth);
  p_type_->set_depth(depth + 1);
}

void TypeAnnot::Print(std::ostream& os) const {
  Node::Print(os);
  p_type_->Print(os);
}

void Component::UpdateDepth(int depth) {
  set_depth(depth);
  p_id_->set_depth(depth + 1);
  p_type_->set_depth(depth + 1);
}

void Component::Print(std::ostream& os) const {
  Node::Print(os);
  p_id_->Print(os);
  p_type_->Print(os);
}

void IdType::UpdateDepth(int depth) {
  set_depth(depth);
  p_id_->set_depth(depth + 1);
}

void IdType::Print(std::ostream& os) const {
  Type::Print(os);
  p_id_->Print(os);
}

void ArrayType::UpdateDepth(int depth) {
  set_depth(depth);
  p_type_->set_depth(depth + 1);
}

void ArrayType::Print(std::ostream& os) const {
  Type::Print(os);
  p_type_->Print(os);
}

void RecordType::UpdateDepth(int depth) {
  set_depth(depth);
  p_components_->set_depth(depth + 1);
}

void RecordType::Print(std::ostream& os) const {
  Type::Print(os);
  p_components_->Print(os);
}
