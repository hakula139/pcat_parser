#ifndef SRC_AST_CONSTANT_HPP_
#define SRC_AST_CONSTANT_HPP_

#include <iostream>
#include <string>
#include <variant>  // std::variant

#include "../base/common.hpp"
#include "../location.hpp"
#include "node.hpp"

template <class T>
class Constant : public Node {
 public:
  explicit Constant(const yy::location& loc, const T& value)
      : Node{loc}, value_{value} {}

  void Print(std::ostream& os) const override {
    PrintBase(os);
    os << " " << value() << "\n";
  }

  std::string name() const override { return name_; }
  virtual T value() const { return value_; }

 private:
  const std::string name_ = "constant";
  const T value_;
};

class Integer : public Constant<int32_t> {
 public:
  explicit Integer(const yy::location& loc, int32_t value)
      : Constant{loc, value} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "integer";
};

class Real : public Constant<double> {
 public:
  explicit Real(const yy::location& loc, double value) : Constant{loc, value} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "real";
};

class Number : public ValueNode {
 public:
  using UnionPtr = std::variant<SPtr<Integer>, SPtr<Real>>;

  explicit Number(const yy::location& loc, UnionPtr p_number)
      : ValueNode{loc}, p_number_{p_number} {}

  void UpdateDepth(int depth) override;

  std::string name() const override { return name_; }
  std::string value() const override;

 private:
  const std::string name_ = "number";
  UnionPtr p_number_;
};

class String : public Constant<std::string> {
 public:
  explicit String(const yy::location& loc, const std::string& value)
      : Constant{loc, value} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "string";
};

#endif  // SRC_AST_CONSTANT_HPP_
