#include "param.hpp"

#include <iostream>

void FormalParam::UpdateDepth(int depth) {
  Param::UpdateDepth(depth);
  if (p_ids_) p_ids_->UpdateDepth(depth + 1);
  if (p_type_) p_type_->UpdateDepth(depth + 1);
}

void FormalParam::Print(std::ostream& os) const {
  Param::Print(os);
  if (p_ids_) p_ids_->Print(os);
  if (p_type_) p_type_->Print(os);
}
