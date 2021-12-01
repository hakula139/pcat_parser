%skeleton "lalr1.cc"
%require "3.8"
%header "src/parser.hpp"
%locations

%define api.parser.class {Parser}
%define api.location.file "location.hpp"

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%define parse.trace
%define parse.error verbose
%define parse.lac full

%lex-param {yy::Lexer* p_lexer}
%parse-param {yy::Lexer* p_lexer} {Driver* p_driver}

%code requires {
#include <string>

#include "ast/index.hpp"
#include "base/common.hpp"

class Driver;
namespace yy {
  class Lexer;
}
}

%code top {
#include <memory>   // std::make_unique
#include <utility>  // std::move

#include "driver.hpp"
#include "lexer.hpp"
#include "utils/logger.hpp"

// Some abbreviations.
#define MOVE()
#define MOVE(x) std::move(x)
#define MOVE(x, ...) std::move(x), MOVE(__VA_ARGS__)
#define MAKE_TOKEN(T, loc, ...) std::make_unique<T>(loc, MOVE(__VA_ARGS__))

static yy::Parser::symbol_type yylex(yy::Lexer* p_lexer) {
  return p_lexer->ReadToken();
}

// A workaround to solve the undefined reference issue, as we have defined
// another yylex() which is not default.
int yyFlexLexer::yylex() {
  Logger::Warn("calling wrong yylex()");
  return EXIT_FAILURE;
}
}

%define api.token.prefix {T_}
%token
  // Reserved keywords
  <std::string>       AND
  <std::string>       ARRAY
  <std::string>       BEGIN
  <std::string>       BY
  <std::string>       DIV
  <std::string>       DO
  <std::string>       ELSE
  <std::string>       ELSIF
  <std::string>       END
  <std::string>       EXIT
  <std::string>       FOR
  <std::string>       IF
  <std::string>       IN
  <std::string>       IS
  <std::string>       LOOP
  <std::string>       MOD
  <std::string>       NOT
  <std::string>       OF
  <std::string>       OR
  <std::string>       OUT
  <std::string>       PROCEDURE
  <std::string>       PROGRAM
  <std::string>       READ
  <std::string>       RECORD
  <std::string>       RETURN
  <std::string>       THEN
  <std::string>       TO
  <std::string>       TYPE
  <std::string>       VAR
  <std::string>       WHILE
  <std::string>       WRITE

  // Operators
  <std::string>       ASSIGN              ":="
  <std::string>       PLUS                "+"
  <std::string>       MINUS               "-"
  <std::string>       STAR                "*"
  <std::string>       SLASH               "/"
  <std::string>       LT                  "<"
  <std::string>       LE                  "<="
  <std::string>       GT                  ">"
  <std::string>       GE                  ">="
  <std::string>       EQ                  "="
  <std::string>       NE                  "<>"

  // Delimiters
  <std::string>       COLON               ":"
  <std::string>       SEMICOLON           ";"
  <std::string>       COMMA               ","
  <std::string>       DOT                 "."
  <std::string>       LPAREN              "("
  <std::string>       RPAREN              ")"
  <std::string>       LSBRAC              "["
  <std::string>       RSBRAC              "]"
  <std::string>       LCBRAC              "{"
  <std::string>       RCBRAC              "}"
  <std::string>       LSABRAC             "[<"
  <std::string>       RSABRAC             ">]"
  <std::string>       BACKSLASH           "\\"

  // Constants
  <UPtr<Integer>>     INTEGER             "integer"
  <UPtr<Real>>        REAL                "real"
  <UPtr<String>>      STRING              "string"

  // Identifiers
  <UPtr<Id>>          ID                  "identifier"
;

%left                 OR;
%left                 AND;
%nonassoc             EQ NE;
%nonassoc             LT LE GT GE;
%left                 PLUS MINUS;
%left                 STAR SLASH DIV MOD;
%right                POS NEG NOT;

%nterm
  // Programs
  <UPtr<Program>>             program
  <UPtr<Body>>                body

  // Declarations
  <UPtr<Decls>>               decls
  <UPtr<Decl>>                decl
  <UPtr<VarDecls>>            var_decls
  <UPtr<VarDecl>>             var_decl
  <UPtr<TypeDecls>>           type_decls
  <UPtr<TypeDecl>>            type_decl
  <UPtr<ProcDecls>>           proc_decls
  <UPtr<ProcDecl>>            proc_decl
  <UPtr<FormalParams>>        formal_params
  <UPtr<FormalParam>>         formal_param
  <UPtr<TypeAnnot>>           type_annot
  <UPtr<Type>>                type
  <UPtr<Components>>          components
  <UPtr<Component>>           component
  <UPtr<Ids>>                 ids

  // Statements
  <UPtr<Stmts>>               stmts
  <UPtr<Stmt>>                stmt
  <UPtr<ActualParams>>        actual_params
  <UPtr<ReadParams>>          read_params
  <UPtr<WriteParams>>         write_params
  <UPtr<ElifSections>>        elif_sections
  <UPtr<ElifSection>>         elif_section
  <UPtr<ElseSection>>         else_section
  <UPtr<ForStep>>             for_step

  // Expressions
  <UPtr<Exprs>>               exprs
  <UPtr<Expr>>                expr
  <UPtr<WriteExprs>>          write_exprs
  <UPtr<WriteExpr>>           write_expr
  <UPtr<AssignExprs>>         assign_exprs
  <UPtr<AssignExpr>>          assign_expr
  <UPtr<ArrayExprs>>          array_exprs
  <UPtr<ArrayExpr>>           array_expr
  <UPtr<Number>>              number
  <UPtr<Lvalues>>             lvalues
  <UPtr<Lvalue>>              lvalue
  <UPtr<CompValues>>          comp_values
  <UPtr<ArrayValues>>         array_values
;

%%
%start program;

// Programs

program:
  PROGRAM IS body SEMICOLON {
    $$ = MAKE_TOKEN(Program, @$, $body);
    p_driver->set_program(MOVE($$));
  }
;

body:
  decls BEGIN stmts END { $$ = MAKE_TOKEN(Body, @$, $decls, $stmts); }
;

// Declarations

decls:
  %empty { $$ = MAKE_TOKEN(Decls, @$); }
| decls decl { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

decl:
  VAR var_decls { $$ = MOVE($2); $$->set_loc(@$); }
| PROCEDURE proc_decls { $$ = MOVE($2); $$->set_loc(@$); }
| TYPE type_decls { $$ = MOVE($2); $$->set_loc(@$); }
;

var_decls:
  var_decl { $$ = MAKE_TOKEN(VarDecls, @$); $$->Insert(MOVE($1)); }
| var_decls var_decl { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

var_decl:
  ids type_annot ASSIGN expr SEMICOLON {
    $$ = MAKE_TOKEN(VarDecl, @$, $ids, $type_annot, $expr);
  }
;

type_decls:
  type_decl { $$ = MAKE_TOKEN(TypeDecls, @$); $$->Insert(MOVE($1)); }
| type_decls type_decl { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

type_decl:
  ID IS type SEMICOLON { $$ = MAKE_TOKEN(TypeDecl, @$, $ID, $type); }
;

proc_decls:
  proc_decl { $$ = MAKE_TOKEN(ProcDecls, @$); $$->Insert(MOVE($1)); }
| proc_decls proc_decl { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

proc_decl:
  ID LPAREN formal_params RPAREN type_annot IS body SEMICOLON {
    $$ = MAKE_TOKEN(ProcDecl, @$, $ID, $formal_params, $type_annot, $body);
  }
;

formal_params:
  %empty { $$ = MAKE_TOKEN(FormalParams, @$); }
| formal_param { $$ = MAKE_TOKEN(FormalParams, @$); $$->Insert(MOVE($1)); }
| formal_params SEMICOLON formal_param { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

formal_param:
  ids COLON type { $$ = MAKE_TOKEN(FormalParam, @$, $ids, $type); }
;

type_annot:
  %empty { $$ = nullptr; }
| COLON type { $$ = MAKE_TOKEN(TypeAnnot, @$, $type); }
;

type:
  ID { $$ = MAKE_TOKEN(IdType, @$, $ID); }
| ARRAY OF type[el_type] { $$ = MAKE_TOKEN(ArrayType, @$, $el_type); }
| RECORD components END { $$ = MAKE_TOKEN(RecordType, @$, $components); }
;

components:
  component { $$ = MAKE_TOKEN(Components, @$); $$->Insert(MOVE($1)); }
| components component { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

component:
  ID COLON type SEMICOLON { $$ = MAKE_TOKEN(Component, @$, $ID, $type); }
;

ids:
  ID { $$ = MAKE_TOKEN(Ids, @$); $$->Insert(MOVE($1)); }
| ids COMMA ID { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

// Statements

stmts:
  %empty { $$ = MAKE_TOKEN(Stmts, @$); }
| stmts stmt { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

stmt:
  lvalue ASSIGN expr SEMICOLON {
    $$ = MAKE_TOKEN(AssignStmt, @$, $lvalue, $expr);
  }
| ID LPAREN actual_params RPAREN SEMICOLON {
    $$ = MAKE_TOKEN(ProcCallStmt, @$, $ID, $actual_params);
  }
| READ LPAREN read_params RPAREN SEMICOLON {
    $$ = MAKE_TOKEN(ReadStmt, @$, $read_params);
  }
| WRITE LPAREN write_params RPAREN SEMICOLON {
    $$ = MAKE_TOKEN(WriteStmt, @$, $write_params);
  }
| IF expr THEN stmts elif_sections else_section END SEMICOLON {
    $$ = MAKE_TOKEN(IfStmt, @$, $expr, $stmts, $elif_sections, $else_section);
  }
| WHILE expr DO stmts END SEMICOLON {
    $$ = MAKE_TOKEN(WhileStmt, @$, $expr, $stmts);
  }
| LOOP stmts END SEMICOLON {
    $$ = MAKE_TOKEN(LoopStmt, @$, $stmts);
  }
| FOR ID ASSIGN expr[begin] TO expr[end] for_step DO stmts END SEMICOLON {
    $$ = MAKE_TOKEN(ForStmt, @$, $ID, $begin, $end, $for_step, $stmts);
  }
| EXIT SEMICOLON {
    $$ = MAKE_TOKEN(ExitStmt, @$);
  }
| RETURN SEMICOLON {
    $$ = MAKE_TOKEN(ReturnStmt, @$);
  }
| RETURN expr SEMICOLON {
    $$ = MAKE_TOKEN(ReturnStmt, @$, $expr);
  }
;

actual_params:
  %empty { $$ = MAKE_TOKEN(ActualParams, @$); }
| exprs { $$ = MAKE_TOKEN(ActualParams, @$); $$->Insert(MOVE($1)); }
;

read_params:
  lvalues { $$ = MAKE_TOKEN(ReadParams, @$); $$->Insert(MOVE($1)); }
;

write_params:
  %empty { $$ = MAKE_TOKEN(WriteParams, @$); }
| write_exprs { $$ = MAKE_TOKEN(WriteParams, @$); $$->Insert(MOVE($1)); }
;

elif_sections:
  %empty { $$ = MAKE_TOKEN(ElifSections, @$); }
| elif_sections elif_section { $$ = MOVE($1); $$->Insert(MOVE($2)); }
;

elif_section:
  ELSIF expr THEN stmts { $$ = MAKE_TOKEN(ElifSection, @$, $expr, $stmts); }
;

else_section:
  %empty { $$ = nullptr; }
| ELSE stmts { $$ = MAKE_TOKEN(ElseSection, @$, $stmts); }
;

for_step:
  %empty { $$ = nullptr; }
| BY expr { $$ = MAKE_TOKEN(ForStep, @$, $expr); }
;

// Expressions

exprs:
  expr { $$ = MAKE_TOKEN(Exprs, @$); $$->Insert(MOVE($1)); }
| exprs COMMA expr { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

expr:
  number { $$ = MAKE_TOKEN(NumberExpr, @$, $1); }
| lvalue { $$ = MAKE_TOKEN(LvalueExpr, @$, $1); }
| LPAREN expr RPAREN { $$ = MAKE_TOKEN(ParenExpr, @$, $2); }
| PLUS expr %prec POS { $$ = MAKE_TOKEN(UnaryExpr, @$, MAKE_TOKEN(Op, @1, $1), $2); }
| MINUS expr %prec NEG { $$ = MAKE_TOKEN(UnaryExpr, @$, MAKE_TOKEN(Op, @1, $1), $2); }
| NOT expr { $$ = MAKE_TOKEN(UnaryExpr, @$, MAKE_TOKEN(Op, @1, $1), $2); }
| expr PLUS expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr MINUS expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr STAR expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr SLASH expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr DIV expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr MOD expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr OR expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr AND expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr LT expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr LE expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr GT expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr GE expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr EQ expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| expr NE expr { $$ = MAKE_TOKEN(BinaryExpr, @$, $1, MAKE_TOKEN(Op, @2, $2), $3); }
| ID LPAREN actual_params RPAREN { $$ = MAKE_TOKEN(ProcCallExpr, @$, $1, $3); }
| ID LCBRAC comp_values RCBRAC { $$ = MAKE_TOKEN(RecordConstrExpr, @$, $1, $3); }
| ID LSABRAC array_values RSABRAC { $$ = MAKE_TOKEN(ArrayConstrExpr, @$, $1, $3); }
;

write_exprs:
  write_expr { $$ = MAKE_TOKEN(WriteExprs, @$); $$->Insert(MOVE($1)); }
| write_exprs COMMA write_expr { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

write_expr:
  STRING { $$ = MAKE_TOKEN(WriteExpr, @$, $1); }
| expr { $$ = MAKE_TOKEN(WriteExpr, @$, $1); }
;

assign_exprs:
  assign_expr { $$ = MAKE_TOKEN(AssignExprs, @$); $$->Insert(MOVE($1)); }
| assign_exprs SEMICOLON assign_expr { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

assign_expr:
  ID ASSIGN expr { $$ = MAKE_TOKEN(AssignExpr, @$, $ID, $expr); }
;

array_exprs:
  array_expr { $$ = MAKE_TOKEN(ArrayExprs, @$); $$->Insert(MOVE($1)); }
| array_exprs COMMA array_expr { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

array_expr:
  expr[v] { $$ = MAKE_TOKEN(ArrayExpr, @$, $v); }
| expr[n] OF expr[v] { $$ = MAKE_TOKEN(ArrayExpr, @$, $v, $n); }
;

number:
  INTEGER { $$ = MAKE_TOKEN(Number, @$, $1); }
| REAL { $$ = MAKE_TOKEN(Number, @$, $1); }
;

lvalues:
  lvalue { $$ = MAKE_TOKEN(Lvalues, @$); $$->Insert(MOVE($1)); }
| lvalues COMMA lvalue { $$ = MOVE($1); $$->Insert(MOVE($3)); }
;

lvalue:
  ID { $$ = MAKE_TOKEN(IdLvalue, @$, $1); }
| lvalue LSBRAC expr RSBRAC { $$ = MAKE_TOKEN(ArrayElemLvalue, @$, $1, $3); }
| lvalue DOT ID { $$ = MAKE_TOKEN(RecordCompLvalue, @$, $1, $3); }
;

comp_values:
  assign_exprs { $$ = MOVE($1); }
;

array_values:
  array_exprs { $$ = MOVE($1); }
;
%%

void yy::Parser::error(const location_type& loc, const std::string& msg) {
  Logger::Error(msg, &loc);
}
