#ifndef SRC_AST_TYPE_HPP_
#define SRC_AST_TYPE_HPP_

#include <iostream>
#include <string>

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"

class Type : public Node {
 public:
  explicit Type(const yy::location& loc) : Node{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "type";
};

class TypeAnnot : public Node {
 public:
  explicit TypeAnnot(const yy::location& loc, SPtr<Type> p_type)
      : Node{loc}, p_type_{p_type} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "type annotation";
  SPtr<Type> p_type_;
};

class Component : public Node {
 public:
  explicit Component(const yy::location& loc, SPtr<Id> p_id, SPtr<Type> p_type)
      : Node{loc}, p_id_{p_id}, p_type_{p_type} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "component";
  SPtr<Id> p_id_;
  SPtr<Type> p_type_;
};

class Components : public Nodes {
 public:
  explicit Components(const yy::location& loc) : Nodes{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "component list";
};

class IdType : public Type {
 public:
  explicit IdType(const yy::location& loc, SPtr<Id> p_id)
      : Type{loc}, p_id_{p_id} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "identifier type";
  SPtr<Id> p_id_;
};

class ArrayType : public Type {
 public:
  explicit ArrayType(const yy::location& loc, SPtr<Type> p_type)
      : Type{loc}, p_type_{p_type} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "array type";
  SPtr<Type> p_type_;
};

class RecordType : public Type {
 public:
  explicit RecordType(const yy::location& loc, SPtr<Components> p_components)
      : Type{loc}, p_components_{p_components} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "record type";
  SPtr<Components> p_components_;
};

#endif  // SRC_AST_TYPE_HPP_
