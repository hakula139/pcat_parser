#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "expr.hpp"
#include "identifier.hpp"
#include "lvalue.hpp"
#include "node.hpp"
#include "param.hpp"

class Stmt;
class Stmts;
class AssignStmt;
class ProcCallStmt;
class ReadStmt;
class WriteStmt;
class IfStmt;
class ElifSection;
class ElifSections;
class ElseSection;
class WhileStmt;
class LoopStmt;
class ForStmt;
class ForStep;
class ExitStmt;
class ReturnStmt;

using StmtPtr = std::unique_ptr<Stmt>;
using StmtsPtr = std::unique_ptr<Stmts>;
using AssignStmtPtr = std::unique_ptr<AssignStmt>;
using ProcCallStmtPtr = std::unique_ptr<ProcCallStmt>;
using ReadStmtPtr = std::unique_ptr<ReadStmt>;
using WriteStmtPtr = std::unique_ptr<WriteStmt>;
using IfStmtPtr = std::unique_ptr<IfStmt>;
using ElifSectionPtr = std::unique_ptr<ElifSection>;
using ElifSectionsPtr = std::unique_ptr<ElifSections>;
using ElseSectionPtr = std::unique_ptr<ElseSection>;
using WhileStmtPtr = std::unique_ptr<WhileStmt>;
using LoopStmtPtr = std::unique_ptr<LoopStmt>;
using ForStmtPtr = std::unique_ptr<ForStmt>;
using ForStepPtr = std::unique_ptr<ForStep>;
using ExitStmtPtr = std::unique_ptr<ExitStmt>;
using ReturnStmtPtr = std::unique_ptr<ReturnStmt>;

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
  std::vector<StmtPtr> data_;
};

class AssignStmt : public Stmt {
 public:
  explicit AssignStmt(
      const yy::location& loc, LvaluePtr p_lvalue, ExprPtr p_expr)
      : Stmt{loc}, p_lvalue_{std::move(p_lvalue)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "assignment statement";
  LvaluePtr p_lvalue_;
  ExprPtr p_expr_;
};

class ProcCallStmt : public Stmt {
 public:
  explicit ProcCallStmt(
      const yy::location& loc, IdPtr p_id, ActualParamsPtr p_actual_params)
      : Stmt{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "procedure call statement";
  IdPtr p_id_;
  ActualParamsPtr p_actual_params_;
};

class ReadStmt : public Stmt {
 public:
  explicit ReadStmt(const yy::location& loc, ReadParamsPtr p_read_params)
      : Stmt{loc}, p_read_params_{std::move(p_read_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "read statement";
  ReadParamsPtr p_read_params_;
};

class WriteStmt : public Stmt {
 public:
  explicit WriteStmt(const yy::location& loc, WriteParamsPtr p_write_params)
      : Stmt{loc}, p_write_params_{std::move(p_write_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "write statement";
  WriteParamsPtr p_write_params_;
};

class IfStmt : public Stmt {
 public:
  explicit IfStmt(
      const yy::location& loc,
      ExprPtr p_expr,
      StmtsPtr p_stmts,
      ElifSectionsPtr p_elif_sections,
      ElseSectionPtr p_else_section)
      : Stmt{loc},
        p_expr_{std::move(p_expr)},
        p_stmts_{std::move(p_stmts)},
        p_elif_sections_{std::move(p_elif_sections)},
        p_else_section_{std::move(p_else_section)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "if statement";
  ExprPtr p_expr_;
  StmtsPtr p_stmts_;
  ElifSectionsPtr p_elif_sections_;
  ElseSectionPtr p_else_section_;
};

class ElifSection : public Node {
 public:
  explicit ElifSection(
      const yy::location& loc, ExprPtr p_expr, StmtsPtr p_stmts)
      : Node{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else if section";
  ExprPtr p_expr_;
  StmtsPtr p_stmts_;
};

class ElifSections : public Nodes {
 public:
  explicit ElifSections(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "else if section list";
  std::vector<ElifSectionPtr> data_;
};

class ElseSection : public Node {
 public:
  explicit ElseSection(const yy::location& loc, StmtsPtr p_stmts)
      : Node{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else section";
  StmtsPtr p_stmts_;
};

class WhileStmt : public Stmt {
 public:
  explicit WhileStmt(const yy::location& loc, ExprPtr p_expr, StmtsPtr p_stmts)
      : Stmt{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "while statement";
  ExprPtr p_expr_;
  StmtsPtr p_stmts_;
};

class LoopStmt : public Stmt {
 public:
  explicit LoopStmt(const yy::location& loc, StmtsPtr p_stmts)
      : Stmt{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "loop statement";
  StmtsPtr p_stmts_;
};

class ForStmt : public Stmt {
 public:
  explicit ForStmt(
      const yy::location& loc,
      IdPtr p_id,
      ExprPtr p_begin,
      ExprPtr p_end,
      ForStepPtr p_step,
      StmtsPtr p_stmts)
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
  IdPtr p_id_;
  ExprPtr p_begin_;
  ExprPtr p_end_;
  ForStepPtr p_step_;
  StmtsPtr p_stmts_;
};

class ForStep : public Node {
 public:
  explicit ForStep(const yy::location& loc, ExprPtr p_expr)
      : Node{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "for step";
  ExprPtr p_expr_;
};

class ExitStmt : public Stmt {
 public:
  explicit ExitStmt(const yy::location& loc) : Stmt{loc} {}

 protected:
  const std::string name_ = "exit statement";
};

class ReturnStmt : public Stmt {
 public:
  explicit ReturnStmt(const yy::location& loc, ExprPtr p_expr)
      : Stmt{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "return statement";
  ExprPtr p_expr_;
};

#endif  // SRC_AST_STMT_HPP_
