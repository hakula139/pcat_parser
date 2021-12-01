#ifndef SRC_AST_STMT_HPP_
#define SRC_AST_STMT_HPP_

#include <iostream>
#include <memory>  // std::unique_ptr
#include <string>
#include <utility>  // std::move
#include <vector>

#include "../location.hpp"
#include "identifier.hpp"
#include "lvalue.hpp"
#include "node.hpp"
#include "param.hpp"

class Stmt : public Node {
 public:
  using Ptr = std::unique_ptr<Stmt>;

  explicit Stmt(const yy::location& loc) : Node{loc} {}

 protected:
  const std::string name_ = "statement";
};

class Stmts : public Nodes {
 public:
  using Ptr = std::unique_ptr<Stmts>;

  explicit Stmts(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "statement list";
  std::vector<Stmt::Ptr> data_;
};

class AssignStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<AssignStmt>;

  explicit AssignStmt(
      const yy::location& loc, Lvalue::Ptr p_lvalue, Expr::Ptr p_expr)
      : Stmt{loc}, p_lvalue_{std::move(p_lvalue)}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "assignment statement";
  Lvalue::Ptr p_lvalue_;
  Expr::Ptr p_expr_;
};

class ProcCallStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<ProcCallStmt>;

  explicit ProcCallStmt(
      const yy::location& loc, Id::Ptr p_id, ActualParams::Ptr p_actual_params)
      : Stmt{loc},
        p_id_{std::move(p_id)},
        p_actual_params_{std::move(p_actual_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "procedure call statement";
  Id::Ptr p_id_;
  ActualParams::Ptr p_actual_params_;
};

class ReadStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<ReadStmt>;

  explicit ReadStmt(const yy::location& loc, ReadParams::Ptr p_read_params)
      : Stmt{loc}, p_read_params_{std::move(p_read_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "read statement";
  ReadParams::Ptr p_read_params_;
};

class WriteStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<WriteStmt>;

  explicit WriteStmt(const yy::location& loc, WriteParams::Ptr p_write_params)
      : Stmt{loc}, p_write_params_{std::move(p_write_params)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "write statement";
  WriteParams::Ptr p_write_params_;
};

class IfStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<IfStmt>;

  explicit IfStmt(
      const yy::location& loc,
      Expr::Ptr p_expr,
      Stmts::Ptr p_stmts,
      ElifSections::Ptr p_elif_sections,
      ElseSection::Ptr p_else_section)
      : Stmt{loc},
        p_expr_{std::move(p_expr)},
        p_stmts_{std::move(p_stmts)},
        p_elif_sections_{std::move(p_elif_sections)},
        p_else_section_{std::move(p_else_section)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "if statement";
  Expr::Ptr p_expr_;
  Stmts::Ptr p_stmts_;
  ElifSections::Ptr p_elif_sections_;
  ElseSection::Ptr p_else_section_;
};

class ElifSection : public Node {
 public:
  using Ptr = std::unique_ptr<ElifSection>;

  explicit ElifSection(
      const yy::location& loc, Expr::Ptr p_expr, Stmts::Ptr p_stmts)
      : Node{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else if section";
  Expr::Ptr p_expr_;
  Stmts::Ptr p_stmts_;
};

class ElifSections : public Nodes {
 public:
  using Ptr = std::unique_ptr<ElifSections>;

  explicit ElifSections(const yy::location& loc) : Nodes{loc} {}

 protected:
  const std::string name_ = "else if section list";
  std::vector<ElifSection::Ptr> data_;
};

class ElseSection : public Node {
 public:
  using Ptr = std::unique_ptr<ElseSection>;

  explicit ElseSection(const yy::location& loc, Stmts::Ptr p_stmts)
      : Node{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "else section";
  Stmts::Ptr p_stmts_;
};

class WhileStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<WhileStmt>;

  explicit WhileStmt(
      const yy::location& loc, Expr::Ptr p_expr, Stmts::Ptr p_stmts)
      : Stmt{loc}, p_expr_{std::move(p_expr)}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "while statement";
  Expr::Ptr p_expr_;
  Stmts::Ptr p_stmts_;
};

class LoopStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<LoopStmt>;

  explicit LoopStmt(const yy::location& loc, Stmts::Ptr p_stmts)
      : Stmt{loc}, p_stmts_{std::move(p_stmts)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "loop statement";
  Stmts::Ptr p_stmts_;
};

class ForStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<ForStmt>;

  explicit ForStmt(
      const yy::location& loc,
      Id::Ptr p_id,
      Expr::Ptr p_begin,
      Expr::Ptr p_end,
      ForStep::Ptr p_step,
      Stmts::Ptr p_stmts)
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
  Id::Ptr p_id_;
  Expr::Ptr p_begin_;
  Expr::Ptr p_end_;
  ForStep::Ptr p_step_;
  Stmts::Ptr p_stmts_;
};

class ForStep : public Node {
 public:
  using Ptr = std::unique_ptr<ForStep>;

  explicit ForStep(const yy::location& loc, Expr::Ptr p_expr)
      : Node{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "for step";
  Expr::Ptr p_expr_;
};

class ExitStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<ExitStmt>;

  explicit ExitStmt(const yy::location& loc) : Stmt{loc} {}

 protected:
  const std::string name_ = "exit statement";
};

class ReturnStmt : public Stmt {
 public:
  using Ptr = std::unique_ptr<ReturnStmt>;

  explicit ReturnStmt(const yy::location& loc, Expr::Ptr p_expr)
      : Stmt{loc}, p_expr_{std::move(p_expr)} {}

  void UpdateDepth(int depth) override;
  void Print(std::ostream& os) const override;

 protected:
  const std::string name_ = "return statement";
  Expr::Ptr p_expr_;
};

#endif  // SRC_AST_STMT_HPP_
