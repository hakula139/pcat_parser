#include "param.hpp"

#include <iostream>

void FormalParam::UpdateDepth(int depth) {
  Param::UpdateDepth(depth);
  p_ids_->UpdateDepth(depth + 1);
  p_type_->UpdateDepth(depth + 1);
}

void FormalParam::Print(std::ostream& os) const {
  Param::Print(os);
  p_ids_->Print(os);
  p_type_->Print(os);
}
