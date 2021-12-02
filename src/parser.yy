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
#include <memory>   // std::make_shared

#include "driver.hpp"
#include "lexer.hpp"
#include "utils/logger.hpp"

using std::make_shared;
using location_type = yy::Parser::location_type;
using symbol_type = yy::Parser::symbol_type;

static symbol_type yylex(yy::Lexer* p_lexer) {
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
  <int32_t>           INTEGER             "integer"
  <double>            REAL                "real"
  <std::string>       STRING              "string"

  // Identifiers
  <std::string>       ID                  "identifier"
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
  <SPtr<Program>>             program
  <SPtr<Body>>                body

  // Declarations
  <SPtr<Decls>>               decls
  <SPtr<Decl>>                decl
  <SPtr<VarDecls>>            var_decls
  <SPtr<VarDecl>>             var_decl
  <SPtr<TypeDecls>>           type_decls
  <SPtr<TypeDecl>>            type_decl
  <SPtr<ProcDecls>>           proc_decls
  <SPtr<ProcDecl>>            proc_decl
  <SPtr<FormalParams>>        formal_params
  <SPtr<FormalParam>>         formal_param
  <SPtr<TypeAnnot>>           type_annot
  <SPtr<Type>>                type
  <SPtr<Components>>          components
  <SPtr<Component>>           component
  <SPtr<Ids>>                 ids
  <SPtr<Id>>                  id

  // Statements
  <SPtr<Stmts>>               stmts
  <SPtr<Stmt>>                stmt
  <SPtr<ActualParams>>        actual_params
  <SPtr<ReadParams>>          read_params
  <SPtr<WriteParams>>         write_params
  <SPtr<ElifSections>>        elif_sections
  <SPtr<ElifSection>>         elif_section
  <SPtr<ElseSection>>         else_section
  <SPtr<ForStep>>             for_step

  // Expressions
  <SPtr<Exprs>>               exprs
  <SPtr<Expr>>                expr
  <SPtr<WriteExprs>>          write_exprs
  <SPtr<WriteExpr>>           write_expr
  <SPtr<AssignExprs>>         assign_exprs
  <SPtr<AssignExpr>>          assign_expr
  <SPtr<ArrayExprs>>          array_exprs
  <SPtr<ArrayExpr>>           array_expr
  <SPtr<Number>>              number
  <SPtr<String>>              string
  <SPtr<Lvalues>>             lvalues
  <SPtr<Lvalue>>              lvalue
  <SPtr<CompValues>>          comp_values
  <SPtr<ArrayValues>>         array_values
;

%%
%start program;

// Programs

program:
  PROGRAM IS body SEMICOLON {
    $$ = make_shared<Program>(@$, $body);
    p_driver->set_program($$);
  }
;

body:
  decls BEGIN stmts END { $$ = make_shared<Body>(@$, $decls, $stmts); }
;

// Declarations

decls:
  %empty { $$ = make_shared<Decls>(@$); }
| decls decl { $$ = $1; if ($$) $$->Insert($2); }
;

decl:
  VAR var_decls { $$ = make_shared<Decl>(@$, $2); }
| PROCEDURE proc_decls { $$ = make_shared<Decl>(@$, $2); }
| TYPE type_decls { $$ = make_shared<Decl>(@$, $2); }
;

var_decls:
  var_decl { $$ = make_shared<VarDecls>(@$); if ($$) $$->Insert($1); }
| var_decls var_decl { $$ = $1; if ($$) $$->Insert($2); }
;

var_decl:
  ids type_annot ASSIGN expr SEMICOLON {
    $$ = make_shared<VarDecl>(@$, $ids, $type_annot, $expr);
  }
;

type_decls:
  type_decl { $$ = make_shared<TypeDecls>(@$); if ($$) $$->Insert($1); }
| type_decls type_decl { $$ = $1; if ($$) $$->Insert($2); }
;

type_decl:
  id IS type SEMICOLON { $$ = make_shared<TypeDecl>(@$, $id, $type); }
;

proc_decls:
  proc_decl { $$ = make_shared<ProcDecls>(@$); if ($$) $$->Insert($1); }
| proc_decls proc_decl { $$ = $1; if ($$) $$->Insert($2); }
;

proc_decl:
  id LPAREN formal_params RPAREN type_annot IS body SEMICOLON {
    $$ = make_shared<ProcDecl>(@$, $id, $formal_params, $type_annot, $body);
  }
;

formal_params:
  %empty { $$ = make_shared<FormalParams>(@$); }
| formal_param { $$ = make_shared<FormalParams>(@$); if ($$) $$->Insert($1); }
| formal_params SEMICOLON formal_param { $$ = $1; if ($$) $$->Insert($3); }
;

formal_param:
  ids COLON type { $$ = make_shared<FormalParam>(@$, $ids, $type); }
;

type_annot:
  %empty { $$ = nullptr; }
| COLON type { $$ = make_shared<TypeAnnot>(@$, $type); }
;

type:
  id { $$ = make_shared<IdType>(@$, $id); }
| ARRAY OF type[el_type] { $$ = make_shared<ArrayType>(@$, $el_type); }
| RECORD components END { $$ = make_shared<RecordType>(@$, $components); }
;

components:
  component { $$ = make_shared<Components>(@$); if ($$) $$->Insert($1); }
| components component { $$ = $1; if ($$) $$->Insert($2); }
;

component:
  id COLON type SEMICOLON { $$ = make_shared<Component>(@$, $id, $type); }
;

ids:
  id { $$ = make_shared<Ids>(@$); if ($$) $$->Insert($1); }
| ids COMMA id { $$ = $1; if ($$) $$->Insert($3); }
;

id:
  ID { $$ = make_shared<Id>(@$, $1); }
;

// Statements

stmts:
  %empty { $$ = make_shared<Stmts>(@$); }
| stmts stmt { $$ = $1; if ($$) $$->Insert($2); }
;

stmt:
  lvalue ASSIGN expr SEMICOLON {
    $$ = make_shared<AssignStmt>(@$, $lvalue, $expr);
  }
| id LPAREN actual_params RPAREN SEMICOLON {
    $$ = make_shared<ProcCallStmt>(@$, $id, $actual_params);
  }
| READ LPAREN read_params RPAREN SEMICOLON {
    $$ = make_shared<ReadStmt>(@$, $read_params);
  }
| WRITE LPAREN write_params RPAREN SEMICOLON {
    $$ = make_shared<WriteStmt>(@$, $write_params);
  }
| IF expr THEN stmts elif_sections[elif] else_section[else] END SEMICOLON {
    $$ = make_shared<IfStmt>(@$, $expr, $stmts, $elif, $else);
  }
| WHILE expr DO stmts END SEMICOLON {
    $$ = make_shared<WhileStmt>(@$, $expr, $stmts);
  }
| LOOP stmts END SEMICOLON {
    $$ = make_shared<LoopStmt>(@$, $stmts);
  }
| FOR id ASSIGN expr[begin] TO expr[end] for_step[step] DO stmts END SEMICOLON {
    $$ = make_shared<ForStmt>(@$, $id, $begin, $end, $step, $stmts);
  }
| EXIT SEMICOLON {
    $$ = make_shared<ExitStmt>(@$);
  }
| RETURN SEMICOLON {
    $$ = make_shared<ReturnStmt>(@$);
  }
| RETURN expr SEMICOLON {
    $$ = make_shared<ReturnStmt>(@$, $expr);
  }
;

actual_params:
  %empty { $$ = make_shared<ActualParams>(@$); }
| exprs { $$ = make_shared<ActualParams>(@$); if ($$) $$->InsertArray($1); }
;

read_params:
  lvalues { $$ = make_shared<ReadParams>(@$); if ($$) $$->InsertArray($1); }
;

write_params:
  %empty { $$ = make_shared<WriteParams>(@$); }
| write_exprs { $$ = make_shared<WriteParams>(@$); if ($$) $$->InsertArray($1); }
;

elif_sections:
  %empty { $$ = make_shared<ElifSections>(@$); }
| elif_sections elif_section { $$ = $1; if ($$) $$->Insert($2); }
;

elif_section:
  ELSIF expr THEN stmts { $$ = make_shared<ElifSection>(@$, $expr, $stmts); }
;

else_section:
  %empty { $$ = nullptr; }
| ELSE stmts { $$ = make_shared<ElseSection>(@$, $stmts); }
;

for_step:
  %empty { $$ = nullptr; }
| BY expr { $$ = make_shared<ForStep>(@$, $expr); }
;

// Expressions

exprs:
  expr { $$ = make_shared<Exprs>(@$); if ($$) $$->Insert($1); }
| exprs COMMA expr { $$ = $1; if ($$) $$->Insert($3); }
;

expr:
  number { $$ = make_shared<NumberExpr>(@$, $1); }
| lvalue { $$ = make_shared<LvalueExpr>(@$, $1); }
| LPAREN expr RPAREN { $$ = make_shared<ParenExpr>(@$, $2); }
| PLUS expr %prec POS { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| MINUS expr %prec NEG { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| NOT expr { $$ = make_shared<UnaryExpr>(@$, make_shared<Op>(@1, $1), $2); }
| expr PLUS expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr MINUS expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr STAR expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr SLASH expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr DIV expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr MOD expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr OR expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr AND expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr LT expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr LE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr GT expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr GE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr EQ expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| expr NE expr { $$ = make_shared<BinaryExpr>(@$, $1, make_shared<Op>(@2, $2), $3); }
| id LPAREN actual_params RPAREN { $$ = make_shared<ProcCallExpr>(@$, $1, $3); }
| id LCBRAC comp_values RCBRAC { $$ = make_shared<RecordConstrExpr>(@$, $1, $3); }
| id LSABRAC array_values RSABRAC { $$ = make_shared<ArrayConstrExpr>(@$, $1, $3); }
;

write_exprs:
  write_expr { $$ = make_shared<WriteExprs>(@$); if ($$) $$->Insert($1); }
| write_exprs COMMA write_expr { $$ = $1; if ($$) $$->Insert($3); }
;

write_expr:
  string { $$ = make_shared<WriteExpr>(@$, $1); }
| expr { $$ = make_shared<WriteExpr>(@$, $1); }
;

assign_exprs:
  assign_expr { $$ = make_shared<AssignExprs>(@$); if ($$) $$->Insert($1); }
| assign_exprs SEMICOLON assign_expr { $$ = $1; if ($$) $$->Insert($3); }
;

assign_expr:
  id ASSIGN expr { $$ = make_shared<AssignExpr>(@$, $id, $expr); }
;

array_exprs:
  array_expr { $$ = make_shared<ArrayExprs>(@$); if ($$) $$->Insert($1); }
| array_exprs COMMA array_expr { $$ = $1; if ($$) $$->Insert($3); }
;

array_expr:
  expr[v] { $$ = make_shared<ArrayExpr>(@$, $v); }
| expr[n] OF expr[v] { $$ = make_shared<ArrayExpr>(@$, $v, $n); }
;

number:
  INTEGER { $$ = make_shared<Integer>(@$, $1); }
| REAL { $$ = make_shared<Real>(@$, $1); }
;

string:
  STRING { $$ = make_shared<String>(@$, $1)); }
;

lvalues:
  lvalue { $$ = make_shared<Lvalues>(@$); if ($$) $$->Insert($1); }
| lvalues COMMA lvalue { $$ = $1; if ($$) $$->Insert($3); }
;

lvalue:
  id { $$ = make_shared<IdLvalue>(@$, $1); }
| lvalue LSBRAC expr RSBRAC { $$ = make_shared<ArrayElemLvalue>(@$, $1, $3); }
| lvalue DOT id { $$ = make_shared<RecordCompLvalue>(@$, $1, $3); }
;

comp_values:
  assign_exprs { $$ = make_shared<CompValues>(@$, $1); }
;

array_values:
  array_exprs { $$ = make_shared<ArrayValues>(@$, $1); }
;
%%

void yy::Parser::error(const location_type& loc, const std::string& msg) {
  Logger::Error(msg, &loc);
}
