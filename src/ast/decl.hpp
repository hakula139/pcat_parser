#ifndef SRC_AST_DECL_HPP_
#define SRC_AST_DECL_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "param.hpp"
#include "type.hpp"

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
  std::vector<UPtr<Decl>> data_;
};

class VarDecl : public Decl {
 public:
  explicit VarDecl(
      const yy::location& loc,
      UPtr<Ids> p_ids,
      UPtr<TypeAnnot> p_type_annot,
      UPtr<Expr> p_expr)
      : Decl{loc},
        p_ids_{std::move(p_ids)},
        p_type_annot_{std::move(p_type_annot)},
        p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "variable declaration";
  UPtr<Ids> p_ids_;
  UPtr<TypeAnnot> p_type_annot_;
  UPtr<Expr> p_expr_;
};

class VarDecls : public Decls {
 public:
  explicit VarDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "variable declaration list";
  std::vector<UPtr<VarDecl>> data_;
};

class TypeDecl : public Decl {
 public:
  explicit TypeDecl(const yy::location& loc, UPtr<Id> p_id, UPtr<Type> p_type)
      : Decl{loc}, p_id_{std::move(p_id)}, p_type_{std::move(p_type)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  UPtr<Id> p_id_;
  UPtr<Type> p_type_;
};

class TypeDecls : public Decls {
 public:
  explicit TypeDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "type declaration list";
  std::vector<UPtr<TypeDecl>> data_;
};

class Body;

class ProcDecl : public Decl {
 public:
  explicit ProcDecl(
      const yy::location& loc,
      UPtr<Id> p_id,
      UPtr<FormalParams> p_formal_params,
      UPtr<TypeAnnot> p_type_annot,
      UPtr<Body> p_body)
      : Decl{loc},
        p_id_{std::move(p_id)},
        p_formal_params_{std::move(p_formal_params)},
        p_type_annot_{std::move(p_type_annot)},
        p_body_{std::move(p_body)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  UPtr<Id> p_id_;
  UPtr<FormalParams> p_formal_params_;
  UPtr<TypeAnnot> p_type_annot_;
  UPtr<Body> p_body_;
};

class ProcDecls : public Decls {
 public:
  explicit ProcDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "procedure declaration list";
  std::vector<UPtr<ProcDecl>> data_;
};

#endif  // SRC_AST_DECL_HPP_
