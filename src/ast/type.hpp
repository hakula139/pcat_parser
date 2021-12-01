#ifndef SRC_AST_TYPE_HPP_
#define SRC_AST_TYPE_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move

#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"

class Type : public Node {
 public:
  using Ptr = std::unique_ptr<Type>;

  explicit Type(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "type";
};

class TypeAnnot : public Node {
 public:
  using Ptr = std::unique_ptr<TypeAnnot>;

  explicit TypeAnnot(const yy::location& loc, Type::Ptr p_type)
      : Node{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type annotation";
  Type::Ptr p_type_;
};

class Component : public Node {
 public:
  using Ptr = std::unique_ptr<Component>;

  explicit Component(const yy::location& loc, Id::Ptr p_id, Type::Ptr p_type)
      : Node{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "component";
  Id::Ptr p_id_;
  Type::Ptr p_type_;
};

class Components : public Nodes {
 public:
  using Ptr = std::unique_ptr<Components>;

  explicit Components(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "component list";
};

class IdType : public Type {
 public:
  using Ptr = std::unique_ptr<IdType>;

  explicit IdType(const yy::location& loc, Id::Ptr p_id)
      : Type{loc}, p_id_{std::move(p_id)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "identifier type";
  Id::Ptr p_id_;
};

class ArrayType : public Type {
 public:
  using Ptr = std::unique_ptr<ArrayType>;

  explicit ArrayType(const yy::location& loc, Type::Ptr p_type)
      : Type{loc}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "array type";
  Type::Ptr p_type_;
};

class RecordType : public Type {
 public:
  using Ptr = std::unique_ptr<RecordType>;

  explicit RecordType(const yy::location& loc, Components::Ptr p_components)
      : Type{loc}, p_components_{std::move(p_components)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "record type";
  Components::Ptr p_components_;
};

#endif  // SRC_AST_TYPE_HPP_
