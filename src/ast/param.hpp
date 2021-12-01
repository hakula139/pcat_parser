#ifndef SRC_AST_PARAM_HPP_
#define SRC_AST_PARAM_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "expr.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "type.hpp"

class Param : public Node {
 public:
  using Ptr = std::unique_ptr<Param>;

  explicit Param(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "parameter";
};

class Params : public Nodes {
 public:
  using Ptr = std::unique_ptr<Params>;

  explicit Params(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "parameter list";
  std::vector<Param::Ptr> data_;
};

class FormalParam : public Param {
 public:
  using Ptr = std::unique_ptr<FormalParam>;

  explicit FormalParam(
      const yy::location& loc, Ids::Ptr p_ids, Type::Ptr p_type)
      : Param{loc}, p_ids_{std::move(p_ids)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "formal parameter";
  Ids::Ptr p_ids_;
  Type::Ptr p_type_;
};

class FormalParams : public Params {
 public:
  using Ptr = std::unique_ptr<FormalParams>;

  explicit FormalParams(const yy::location& loc) : Params{loc} {}

 protected:
  const std::string name_ = "formal parameter list";
  std::vector<FormalParam::Ptr> data_;
};

class ActualParams : public Exprs {
 public:
  using Ptr = std::unique_ptr<ActualParams>;

  explicit ActualParams(const yy::location& loc) : Exprs{loc} {}

 protected:
  const std::string name_ = "actual parameter list";
};

class ReadParams : public Lvalues {
 public:
  using Ptr = std::unique_ptr<ReadParams>;

  explicit ReadParams(const yy::location& loc) : Lvalues{loc} {}

 protected:
  const std::string name_ = "read parameter list";
};

class WriteParams : public WriteExprs {
 public:
  using Ptr = std::unique_ptr<WriteParams>;

  explicit WriteParams(const yy::location& loc) : WriteExprs{loc} {}

 protected:
  const std::string name_ = "write parameter list";
};

#endif  // SRC_AST_PARAM_HPP_
