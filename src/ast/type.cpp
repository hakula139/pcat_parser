#include "type.hpp"

#include <iostream>

void TypeAnnot::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_type_->UpdateDepth(depth + 1);
}

void TypeAnnot::Print(std::ostream& os) const {
  Node::Print(os);
  p_type_->Print(os);
}

void Component::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
  p_type_->UpdateDepth(depth + 1);
}

void Component::Print(std::ostream& os) const {
  Node::Print(os);
  p_id_->Print(os);
  p_type_->Print(os);
}

void IdType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  p_id_->UpdateDepth(depth + 1);
}

void IdType::Print(std::ostream& os) const {
  Type::Print(os);
  p_id_->Print(os);
}

void ArrayType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  p_type_->UpdateDepth(depth + 1);
}

void ArrayType::Print(std::ostream& os) const {
  Type::Print(os);
  p_type_->Print(os);
}

void RecordType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  p_components_->UpdateDepth(depth + 1);
}

void RecordType::Print(std::ostream& os) const {
  Type::Print(os);
  p_components_->Print(os);
}
