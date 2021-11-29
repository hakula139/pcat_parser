#include "param.hpp"

#include <iostream>

void FormalParam::UpdateDepth(int depth) {
  set_depth(depth);
  p_ids_->set_depth(depth + 1);
  p_type_->set_depth(depth + 1);
}

void FormalParam::Print(std::ostream& os) const {
  Param::Print(os);
  p_ids_->Print(os);
  p_type_->Print(os);
}
