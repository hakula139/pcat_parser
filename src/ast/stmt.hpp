#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <string>
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

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "statement";
};

class Stmts : public Nodes {
 public:
  explicit Stmts(const yy::location& loc) : Nodes{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "statement list";
  std::vector<SPtr<Stmt>> data_;
};

class AssignStmt : public Stmt {
 public:
  explicit AssignStmt(
      const yy::location& loc, SPtr<Lvalue> p_lvalue, SPtr<Expr> p_expr)
      : Stmt{loc}, p_lvalue_{p_lvalue}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "assignment statement";
  SPtr<Lvalue> p_lvalue_;
  SPtr<Expr> p_expr_;
};

class ProcCallStmt : public Stmt {
 public:
  explicit ProcCallStmt(
      const yy::location& loc,
      SPtr<Id> p_id,
      SPtr<ActualParams> p_actual_params)
      : Stmt{loc}, p_id_{p_id}, p_actual_params_{p_actual_params} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "procedure call statement";
  SPtr<Id> p_id_;
  SPtr<ActualParams> p_actual_params_;
};

class ReadStmt : public Stmt {
 public:
  explicit ReadStmt(const yy::location& loc, SPtr<ReadParams> p_read_params)
      : Stmt{loc}, p_read_params_{p_read_params} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "read statement";
  SPtr<ReadParams> p_read_params_;
};

class WriteStmt : public Stmt {
 public:
  explicit WriteStmt(const yy::location& loc, SPtr<WriteParams> p_write_params)
      : Stmt{loc}, p_write_params_{p_write_params} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "write statement";
  SPtr<WriteParams> p_write_params_;
};

class ElifSection : public Node {
 public:
  explicit ElifSection(
      const yy::location& loc, SPtr<Expr> p_expr, SPtr<Stmts> p_stmts)
      : Node{loc}, p_expr_{p_expr}, p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "else if section";
  SPtr<Expr> p_expr_;
  SPtr<Stmts> p_stmts_;
};

class ElifSections : public Nodes {
 public:
  explicit ElifSections(const yy::location& loc) : Nodes{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "else if section list";
  std::vector<SPtr<ElifSection>> data_;
};

class ElseSection : public Node {
 public:
  explicit ElseSection(const yy::location& loc, SPtr<Stmts> p_stmts)
      : Node{loc}, p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "else section";
  SPtr<Stmts> p_stmts_;
};

class IfStmt : public Stmt {
 public:
  explicit IfStmt(
      const yy::location& loc,
      SPtr<Expr> p_expr,
      SPtr<Stmts> p_stmts,
      SPtr<ElifSections> p_elif_sections,
      SPtr<ElseSection> p_else_section)
      : Stmt{loc},
        p_expr_{p_expr},
        p_stmts_{p_stmts},
        p_elif_sections_{p_elif_sections},
        p_else_section_{p_else_section} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "if statement";
  SPtr<Expr> p_expr_;
  SPtr<Stmts> p_stmts_;
  SPtr<ElifSections> p_elif_sections_;
  SPtr<ElseSection> p_else_section_;
};

class WhileStmt : public Stmt {
 public:
  explicit WhileStmt(
      const yy::location& loc, SPtr<Expr> p_expr, SPtr<Stmts> p_stmts)
      : Stmt{loc}, p_expr_{p_expr}, p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "while statement";
  SPtr<Expr> p_expr_;
  SPtr<Stmts> p_stmts_;
};

class LoopStmt : public Stmt {
 public:
  explicit LoopStmt(const yy::location& loc, SPtr<Stmts> p_stmts)
      : Stmt{loc}, p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "loop statement";
  SPtr<Stmts> p_stmts_;
};

class ForStep : public Node {
 public:
  explicit ForStep(const yy::location& loc, SPtr<Expr> p_expr)
      : Node{loc}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "for step";
  SPtr<Expr> p_expr_;
};

class ForStmt : public Stmt {
 public:
  explicit ForStmt(
      const yy::location& loc,
      SPtr<Id> p_id,
      SPtr<Expr> p_begin,
      SPtr<Expr> p_end,
      SPtr<ForStep> p_step,
      SPtr<Stmts> p_stmts)
      : Stmt{loc},
        p_id_{p_id},
        p_begin_{p_begin},
        p_end_{p_end},
        p_step_{p_step},
        p_stmts_{p_stmts} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "for statement";
  SPtr<Id> p_id_;
  SPtr<Expr> p_begin_;
  SPtr<Expr> p_end_;
  SPtr<ForStep> p_step_;
  SPtr<Stmts> p_stmts_;
};

class ExitStmt : public Stmt {
 public:
  explicit ExitStmt(const yy::location& loc) : Stmt{loc} {}

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "exit statement";
};

class ReturnStmt : public Stmt {
 public:
  explicit ReturnStmt(const yy::location& loc, SPtr<Expr> p_expr = nullptr)
      : Stmt{loc}, p_expr_{p_expr} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

  std::string name() const override { return name_; }

 private:
  const std::string name_ = "return statement";
  SPtr<Expr> p_expr_;
};

#endif  // SRC_AST_STMT_HPP_
