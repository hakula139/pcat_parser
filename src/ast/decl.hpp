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

class Decl : public Node {
 public:
  using Ptr = std::unique_ptr<Decl>;

  explicit Decl(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "declaration";
};

class Decls : public Nodes {
 public:
  using Ptr = std::unique_ptr<Decls>;

  explicit Decls(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "declaration list";
  std::vector<Decl::Ptr> data_;
};

class VarDecl : public Decl {
 public:
  using Ptr = std::unique_ptr<VarDecl>;

  explicit VarDecl(
      const yy::location& loc,
      Ids::Ptr p_ids,
      TypeAnnot::Ptr p_type_annot,
      Expr::Ptr p_expr)
      : Decl{loc},
        p_ids_{std::move(p_ids)},
        p_type_annot_{std::move(p_type_annot)},
        p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "variable declaration";
  Ids::Ptr p_ids_;
  TypeAnnot::Ptr p_type_annot_;
  Expr::Ptr p_expr_;
};

class VarDecls : public Decls {
 public:
  using Ptr = std::unique_ptr<VarDecls>;

  explicit VarDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "variable declaration list";
  std::vector<VarDecl::Ptr> data_;
};

class TypeDecl : public Decl {
 public:
  using Ptr = std::unique_ptr<TypeDecl>;

  explicit TypeDecl(const yy::location& loc, Id::Ptr p_id, Type::Ptr p_type)
      : Decl{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  Id::Ptr p_id_;
  Type::Ptr p_type_;
};

class TypeDecls : public Decls {
 public:
  using Ptr = std::unique_ptr<TypeDecls>;

  explicit TypeDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "type declaration list";
  std::vector<TypeDecl::Ptr> data_;
};

class ProcDecl : public Decl {
 public:
  using Ptr = std::unique_ptr<ProcDecl>;

  explicit ProcDecl(
      const yy::location& loc,
      Id::Ptr p_id,
      FormalParams::Ptr p_formal_params,
      TypeAnnot::Ptr p_type_annot,
      Body::Ptr p_body)
      : Decl{loc},
        p_id_{std::move(p_id)},
        p_formal_params_{std::move(p_formal_params)},
        p_type_annot_{std::move(p_type_annot)},
        p_body_{std::move(p_body)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  Id::Ptr p_id_;
  FormalParams::Ptr p_formal_params_;
  TypeAnnot::Ptr p_type_annot_;
  Body::Ptr p_body_;
};

class ProcDecls : public Decls {
 public:
  using Ptr = std::unique_ptr<ProcDecls>;

  explicit ProcDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "procedure declaration list";
  std::vector<ProcDecl::Ptr> data_;
};

#endif  // SRC_AST_DECL_HPP_
