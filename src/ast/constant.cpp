#include "constant.hpp"

#include <iostream>
#include <string>

#include "../base/common.hpp"

void Number::UpdateDepth(int depth) {
  Expr::UpdateDepth(depth);
  auto visitor = Overloaded{
      [depth](auto&& p) {
        if (p) p->UpdateDepth(depth + 1);
      },
  };
  std::visit(visitor, p_number_);
}

std::string Number::value() const {
  auto visitor = Overloaded{
      [](const auto& p) {
        auto value = p ? std::to_string(p->value()) : "";
        return value;
      },
  };
  return std::visit(visitor, p_number_);
}
