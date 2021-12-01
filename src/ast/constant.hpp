#ifndef SRC_AST_CONSTANT_HPP_
#define SRC_AST_CONSTANT_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <variant>  // std::variant

#include "../location.hpp"
#include "expr.hpp"
#include "node.hpp"

template <class T>
class Constant;
class Integer;
class Real;
class Number;
class String;

template <class T>
using ConstantPtr = std::unique_ptr<Constant<T>>;
using IntegerPtr = std::unique_ptr<Integer>;
using RealPtr = std::unique_ptr<Real>;
using NumberPtr = std::unique_ptr<Number>;
using StringPtr = std::unique_ptr<String>;

template <class T>
class Constant : public Node {
 public:
  explicit Constant(const yy::location& loc, const T& value)
      : Node{loc}, value_{value} {}

  void Print(std::ostream& os) const override {
    PrintBase(os);
    os << " " << value() << "\n";
  }
  virtual T value() const { return value_; }

 protected:
  const std::string name_ = "constant";
  const T value_;
};

class Integer : public Constant<int32_t> {
 public:
  explicit Integer(const yy::location& loc, int32_t value)
      : Constant{loc, value} {}

 protected:
  const std::string name_ = "integer";
};

class Real : public Constant<double> {
 public:
  explicit Real(const yy::location& loc, double value) : Constant{loc, value} {}

 protected:
  const std::string name_ = "real";
};

class Number : public Expr {
  using UnionPtr = std::variant<IntegerPtr, RealPtr>;

 public:
  explicit Number(const yy::location& loc, UnionPtr p_number)
      : Expr{loc}, p_number_{std::move(p_number)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "number";
  UnionPtr p_number_;
};

class String : public Constant<std::string> {
 public:
  explicit String(const yy::location& loc, const std::string& value)
      : Constant{loc, value} {}

 protected:
  const std::string name_ = "string";
};

#endif  // SRC_AST_CONSTANT_HPP_
