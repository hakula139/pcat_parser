#ifndef SRC_AST_TYPE_HPP_
#define SRC_AST_TYPE_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"

class Type : public Node {
 public:
  explicit Type(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "type";
};

class TypeAnnot : public Node {
 public:
  explicit TypeAnnot(const yy::location& loc, UPtr<Type> p_type)
      : Node{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type annotation";
  UPtr<Type> p_type_;
};

class Component : public Node {
 public:
  explicit Component(const yy::location& loc, UPtr<Id> p_id, UPtr<Type> p_type)
      : Node{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "component";
  UPtr<Id> p_id_;
  UPtr<Type> p_type_;
};

class Components : public Nodes {
 public:
  explicit Components(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "component list";
};

class IdType : public Type {
 public:
  explicit IdType(const yy::location& loc, UPtr<Id> p_id)
      : Type{loc}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "identifier type";
  UPtr<Id> p_id_;
};

class ArrayType : public Type {
 public:
  explicit ArrayType(const yy::location& loc, UPtr<Type> p_type)
      : Type{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "array type";
  UPtr<Type> p_type_;
};

class RecordType : public Type {
 public:
  explicit RecordType(const yy::location& loc, UPtr<Components> p_components)
      : Type{loc}, p_components_{std::move(p_components)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "record type";
  UPtr<Components> p_components_;
};

#endif  // SRC_AST_TYPE_HPP_
