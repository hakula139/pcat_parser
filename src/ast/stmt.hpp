#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../base/common.hpp"
#include "../location.hpp"
#include "identifier.hpp"
#include "lvalue.hpp"
#include "node.hpp"
#include "param.hpp"

class Stmt : public Node {
 public:
  explicit Stmt(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "statement";
};

class Stmts : public Nodes {
 public:
  explicit Stmts(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "statement list";
  std::vector<UPtr<Stmt>> data_;
};

class AssignStmt : public Stmt {
 public:
  explicit AssignStmt(
      const yy::location& loc, UPtr<Lvalue> p_lvalue, UPtr<Expr> p_expr)
      : Stmt{loc}, p_lvalue_{std::move(p_lvalue)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "assignment statement";
  UPtr<Lvalue> p_lvalue_;
  UPtr<Expr> p_expr_;
};

class ProcCallStmt : public Stmt {
 public:
  explicit ProcCallStmt(
      const yy::location& loc,
      UPtr<Id> p_id,
      UPtr<ActualParams> p_actual_params)
      : Stmt{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "procedure call statement";
  UPtr<Id> p_id_;
  UPtr<ActualParams> p_actual_params_;
};

class ReadStmt : public Stmt {
 public:
  explicit ReadStmt(const yy::location& loc, UPtr<ReadParams> p_read_params)
      : Stmt{loc}, p_read_params_{std::move(p_read_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "read statement";
  UPtr<ReadParams> p_read_params_;
};

class WriteStmt : public Stmt {
 public:
  explicit WriteStmt(const yy::location& loc, UPtr<WriteParams> p_write_params)
      : Stmt{loc}, p_write_params_{std::move(p_write_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "write statement";
  UPtr<WriteParams> p_write_params_;
};

class ElifSection : public Node {
 public:
  explicit ElifSection(
      const yy::location& loc, UPtr<Expr> p_expr, UPtr<Stmts> p_stmts)
      : Node{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else if section";
  UPtr<Expr> p_expr_;
  UPtr<Stmts> p_stmts_;
};

class ElifSections : public Nodes {
 public:
  explicit ElifSections(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "else if section list";
  std::vector<UPtr<ElifSection>> data_;
};

class ElseSection : public Node {
 public:
  explicit ElseSection(const yy::location& loc, UPtr<Stmts> p_stmts)
      : Node{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else section";
  UPtr<Stmts> p_stmts_;
};

class IfStmt : public Stmt {
 public:
  explicit IfStmt(
      const yy::location& loc,
      UPtr<Expr> p_expr,
      UPtr<Stmts> p_stmts,
      UPtr<ElifSections> p_elif_sections,
      UPtr<ElseSection> p_else_section)
      : Stmt{loc},
        p_expr_{std::move(p_expr)},
        p_stmts_{std::move(p_stmts)},
        p_elif_sections_{std::move(p_elif_sections)},
        p_else_section_{std::move(p_else_section)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "if statement";
  UPtr<Expr> p_expr_;
  UPtr<Stmts> p_stmts_;
  UPtr<ElifSections> p_elif_sections_;
  UPtr<ElseSection> p_else_section_;
};

class WhileStmt : public Stmt {
 public:
  explicit WhileStmt(
      const yy::location& loc, UPtr<Expr> p_expr, UPtr<Stmts> p_stmts)
      : Stmt{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "while statement";
  UPtr<Expr> p_expr_;
  UPtr<Stmts> p_stmts_;
};

class LoopStmt : public Stmt {
 public:
  explicit LoopStmt(const yy::location& loc, UPtr<Stmts> p_stmts)
      : Stmt{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "loop statement";
  UPtr<Stmts> p_stmts_;
};

class ForStep : public Node {
 public:
  explicit ForStep(const yy::location& loc, UPtr<Expr> p_expr)
      : Node{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "for step";
  UPtr<Expr> p_expr_;
};

class ForStmt : public Stmt {
 public:
  explicit ForStmt(
      const yy::location& loc,
      UPtr<Id> p_id,
      UPtr<Expr> p_begin,
      UPtr<Expr> p_end,
      UPtr<ForStep> p_step,
      UPtr<Stmts> p_stmts)
      : Stmt{loc},
        p_id_{std::move(p_id)},
        p_begin_{std::move(p_begin)},
        p_end_{std::move(p_end)},
        p_step_{std::move(p_step)},
        p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "for statement";
  UPtr<Id> p_id_;
  UPtr<Expr> p_begin_;
  UPtr<Expr> p_end_;
  UPtr<ForStep> p_step_;
  UPtr<Stmts> p_stmts_;
};

class ExitStmt : public Stmt {
 public:
  explicit ExitStmt(const yy::location& loc) : Stmt{loc} {}

 protected:
  const std::string name_ = "exit statement";
};

class ReturnStmt : public Stmt {
 public:
  explicit ReturnStmt(const yy::location& loc, UPtr<Expr> p_expr = nullptr)
      : Stmt{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "return statement";
  UPtr<Expr> p_expr_;
};

#endif  // SRC_AST_STMT_HPP_
