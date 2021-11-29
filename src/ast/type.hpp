#ifndef SRC_AST_TYPE_HPP_
#define SRC_AST_TYPE_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "node.hpp"

class Type;
class TypeAnnot;
class Component;
class Components;
class IdType;
class ArrayType;
class RecordType;

using TypePtr = std::unique_ptr<Type>;
using TypeAnnotPtr = std::unique_ptr<TypeAnnot>;
using ComponentPtr = std::unique_ptr<Component>;
using ComponentsPtr = std::unique_ptr<Components>;
using IdTypePtr = std::unique_ptr<IdType>;
using ArrayTypePtr = std::unique_ptr<ArrayType>;
using RecordTypePtr = std::unique_ptr<RecordType>;

class Type : public Node {
 public:
  explicit Type(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "type";
};

class TypeAnnot : public Node {
 public:
  explicit TypeAnnot(const yy::location& loc, TypePtr p_type)
      : Node{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type annotation";
  TypePtr p_type_;
};

class Component : public Node {
 public:
  explicit Component(const yy::location& loc, IdPtr p_id, TypePtr p_type)
      : Node{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "component";
  IdPtr p_id_;
  TypePtr p_type_;
};

class Components : public Nodes {
 public:
  explicit Components(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "component list";
};

class IdType : public Type {
 public:
  explicit IdType(const yy::location& loc, IdPtr p_id)
      : Type{loc}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "identifier type";
  IdPtr p_id_;
};

class ArrayType : public Type {
 public:
  explicit ArrayType(const yy::location& loc, TypePtr p_type)
      : Type{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "array type";
  TypePtr p_type_;
};

class RecordType : public Type {
 public:
  explicit RecordType(const yy::location& loc, ComponentsPtr p_components)
      : Type{loc}, p_components_{std::move(p_components)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "record type";
  ComponentsPtr p_components_;
};

#endif  // SRC_AST_TYPE_HPP_
