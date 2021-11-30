#include "type.hpp"

#include <iostream>

void TypeAnnot::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_type_) p_type_->UpdateDepth(depth + 1);
}

void TypeAnnot::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_type_) p_type_->Print(os);
}

void Component::UpdateDepth(int depth) {
  Node::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
  if (p_type_) p_type_->UpdateDepth(depth + 1);
}

void Component::Print(std::ostream& os) const {
  Node::Print(os);
  if (p_id_) p_id_->Print(os);
  if (p_type_) p_type_->Print(os);
}

void IdType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  if (p_id_) p_id_->UpdateDepth(depth + 1);
}

void IdType::Print(std::ostream& os) const {
  Type::Print(os);
  if (p_id_) p_id_->Print(os);
}

void ArrayType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  if (p_type_) p_type_->UpdateDepth(depth + 1);
}

void ArrayType::Print(std::ostream& os) const {
  Type::Print(os);
  if (p_type_) p_type_->Print(os);
}

void RecordType::UpdateDepth(int depth) {
  Type::UpdateDepth(depth);
  if (p_components_) p_components_->UpdateDepth(depth + 1);
}

void RecordType::Print(std::ostream& os) const {
  Type::Print(os);
  if (p_components_) p_components_->Print(os);
}
