#ifndef SRC_AST_CONSTANT_HPP_
#define SRC_AST_CONSTANT_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "node.hpp"

template <class T>
class Number;
class Integer;
class Real;
class String;

template <class T>
using NumberPtr = std::unique_ptr<Number<T>>;
using IntegerPtr = std::unique_ptr<Integer>;
using RealPtr = std::unique_ptr<Real>;
using StringPtr = std::unique_ptr<String>;

template <class T>
class Number : public Terminal<T> {
 public:
  explicit Number(const yy::location& loc, T value) : Terminal{loc, value} {}

 protected:
  const std::string name_ = "number";
};

class Integer : public Number<int32_t> {
 public:
  explicit Integer(const yy::location& loc, int32_t value)
      : Number{loc, value} {}

 protected:
  const std::string name_ = "integer";
};

class Real : public Number<double> {
 public:
  explicit Real(const yy::location& loc, double value) : Number{loc, value} {}

 protected:
  const std::string name_ = "real";
};

class String : public Terminal<std::string> {
 public:
  explicit Real(const yy::location& loc, const std::string& value)
      : Terminal{loc, value} {}

 protected:
  const std::string name_ = "string";
};

#endif  // SRC_AST_CONSTANT_HPP_
