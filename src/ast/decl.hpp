#ifndef SRC_AST_DECL_HPP_
#define SRC_AST_DECL_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "param.hpp"
#include "type.hpp"

class Decl;
class Decls;
class VarDecl;
class VarDecls;
class TypeDecl;
class TypeDecls;
class ProcDecl;
class ProcDecls;

class Body;

using DeclPtr = std::unique_ptr<Decl>;
using DeclsPtr = std::unique_ptr<Decls>;
using VarDeclPtr = std::unique_ptr<VarDecl>;
using VarDeclsPtr = std::unique_ptr<VarDecls>;
using TypeDeclPtr = std::unique_ptr<TypeDecl>;
using TypeDeclsPtr = std::unique_ptr<TypeDecls>;
using ProcDeclPtr = std::unique_ptr<ProcDecl>;
using ProcDeclsPtr = std::unique_ptr<ProcDecls>;

class Decl : public Node {
 public:
  explicit Decl(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "declaration";
};

class Decls : public Nodes {
 public:
  explicit Decls(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "declaration list";
  std::vector<DeclPtr> data_;
};

class VarDecl : public Decl {
 public:
  explicit VarDecl(
      const yy::location& loc,
      IdsPtr p_ids,
      TypeAnnotPtr p_type_annot,
      ExprPtr p_expr)
      : Decl{loc},
        p_ids_{std::move(p_ids)},
        p_type_annot_{std::move(p_type_annot)},
        p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "variable declaration";
  IdsPtr p_ids_;
  TypeAnnotPtr p_type_annot_;
  ExprPtr p_expr_;
};

class VarDecls : public Decls {
 public:
  explicit VarDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "variable declaration list";
  std::vector<VarDeclPtr> data_;
};

class TypeDecl : public Decl {
 public:
  explicit TypeDecl(const yy::location& loc, IdPtr p_id, TypePtr p_type)
      : Decl{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  IdPtr p_id_;
  TypePtr p_type_;
};

class TypeDecls : public Decls {
 public:
  explicit TypeDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "type declaration list";
  std::vector<TypeDeclPtr> data_;
};

class ProcDecl : public Decl {
 public:
  explicit ProcDecl(
      const yy::location& loc,
      IdPtr p_id,
      FormalParamsPtr p_formal_params,
      TypeAnnotPtr p_type_annot,
      BodyPtr p_body)
      : Decl{loc},
        p_id_{std::move(p_id)},
        p_formal_params_{std::move(p_formal_params)},
        p_type_annot_{std::move(p_type_annot)},
        p_body_{std::move(p_body)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  IdPtr p_id_;
  FormalParamsPtr p_formal_params_;
  TypeAnnotPtr p_type_annot_;
  BodyPtr p_body_;
};

class ProcDecls : public Decls {
 public:
  explicit ProcDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "procedure declaration list";
  std::vector<ProcDeclPtr> data_;
};

#endif  // SRC_AST_DECL_HPP_
