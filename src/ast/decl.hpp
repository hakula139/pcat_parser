#ifndef SRC_AST_DECL_HPP_
#define SRC_AST_DECL_HPP_

#include <iostream>
#include <string>
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "node.hpp"
#include "param.hpp"
#include "type.hpp"

class Decls;

class Decl : public Node {
 public:
  explicit Decl(const yy::location& loc, SPtr<Decls> p_decls = nullptr)
      : Node{loc}, p_decls_{p_decls} {}

 protected:
  const std::string name_ = "declaration";
  SPtr<Decls> p_decls_;
};

class Decls : public Nodes {
 public:
  explicit Decls(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "declaration list";
  std::vector<SPtr<Decl>> data_;
};

class VarDecl : public Decl {
 public:
  explicit VarDecl(
      const yy::location& loc,
      SPtr<Ids> p_ids,
      SPtr<TypeAnnot> p_type_annot,
      SPtr<Expr> p_expr)
      : Decl{loc},
        p_ids_{p_ids},
        p_type_annot_{p_type_annot},
        p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "variable declaration";
  SPtr<Ids> p_ids_;
  SPtr<TypeAnnot> p_type_annot_;
  SPtr<Expr> p_expr_;
};

class VarDecls : public Decls {
 public:
  explicit VarDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "variable declaration list";
  std::vector<SPtr<VarDecl>> data_;
};

class TypeDecl : public Decl {
 public:
  explicit TypeDecl(const yy::location& loc, SPtr<Id> p_id, SPtr<Type> p_type)
      : Decl{loc}, p_id_{p_id}, p_type_{p_type} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  SPtr<Id> p_id_;
  SPtr<Type> p_type_;
};

class TypeDecls : public Decls {
 public:
  explicit TypeDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "type declaration list";
  std::vector<SPtr<TypeDecl>> data_;
};

class Body;

class ProcDecl : public Decl {
 public:
  explicit ProcDecl(
      const yy::location& loc,
      SPtr<Id> p_id,
      SPtr<FormalParams> p_formal_params,
      SPtr<TypeAnnot> p_type_annot,
      SPtr<Body> p_body)
      : Decl{loc},
        p_id_{p_id},
        p_formal_params_{p_formal_params},
        p_type_annot_{p_type_annot},
        p_body_{p_body} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "type declaration";
  SPtr<Id> p_id_;
  SPtr<FormalParams> p_formal_params_;
  SPtr<TypeAnnot> p_type_annot_;
  SPtr<Body> p_body_;
};

class ProcDecls : public Decls {
 public:
  explicit ProcDecls(const yy::location& loc) : Decls{loc} {}

 protected:
  const std::string name_ = "procedure declaration list";
  std::vector<SPtr<ProcDecl>> data_;
};

#endif  // SRC_AST_DECL_HPP_
