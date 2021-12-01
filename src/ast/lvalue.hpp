#ifndef SRC_AST_LVALUE_HPP_
#define SRC_AST_LVALUE_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "expr.hpp"
#include "identifier.hpp"
#include "node.hpp"

class Lvalue : public Expr {
 public:
  using Ptr = std::unique_ptr<Lvalue>;

  explicit Lvalue(const yy::location& loc) : Expr{loc} {}

 protected:
  const std::string name_ = "lvalue";
};

class Lvalues : public Nodes {
 public:
  using Ptr = std::unique_ptr<Lvalues>;

  explicit Lvalues(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "lvalue list";
  std::vector<Lvalue::Ptr> data_;
};

class IdLvalue : public Lvalue {
 public:
  using Ptr = std::unique_ptr<IdLvalue>;

  explicit IdLvalue(const yy::location& loc, Id::Ptr p_id)
      : Lvalue{loc}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "identifier lvalue";
  Id::Ptr p_id_;
};

class ArrayElemLvalue : public Lvalue {
 public:
  using Ptr = std::unique_ptr<ArrayElemLvalue>;

  explicit ArrayElemLvalue(
      const yy::location& loc, Lvalue::Ptr p_lvalue, Expr::Ptr p_expr)
      : Lvalue{loc},
        p_lvalue_{std::move(p_lvalue)},
        p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "array element lvalue";
  Lvalue::Ptr p_lvalue_;
  Expr::Ptr p_expr_;
};

class RecordCompLvalue : public Lvalue {
 public:
  using Ptr = std::unique_ptr<RecordCompLvalue>;

  explicit RecordCompLvalue(
      const yy::location& loc, Lvalue::Ptr p_lvalue, Id::Ptr p_id)
      : Lvalue{loc}, p_lvalue_{std::move(p_lvalue)}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  std::string value() const override;

 protected:
  const std::string name_ = "record component lvalue";
  Lvalue::Ptr p_lvalue_;
  Id::Ptr p_id_;
};

#endif  // SRC_AST_LVALUE_HPP_
