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
%parse-param {yy::Lexer* p_lexer}

%code requires {
#include <string>

#include "ast/index.hpp"

class Driver;
namespace yy {
  class Lexer;
}
}

%code top {
#include "driver.hpp"
#include "lexer.hpp"
#include "utils/logger.hpp"

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
  AND
  ARRAY
  BEGIN
  BY
  DIV
  DO
  ELSE
  ELSIF
  END
  EXIT
  FOR
  IF
  IN
  IS
  LOOP
  MOD
  NOT
  OF
  OR
  OUT
  PROCEDURE
  PROGRAM
  READ
  RECORD
  RETURN
  THEN
  TO
  TYPE
  VAR
  WHILE
  WRITE

  // Operators
  ASSIGN              ":="
  PLUS                "+"
  MINUS               "-"
  STAR                "*"
  SLASH               "/"
  LT                  "<"
  LE                  "<="
  GT                  ">"
  GE                  ">="
  EQ                  "="
  NE                  "<>"

  // Delimiters
  COLON               ":"
  SEMICOLON           ";"
  COMMA               ","
  DOT                 "."
  LPAREN              "("
  RPAREN              ")"
  LSBRAC              "["
  RSBRAC              "]"
  LCBRAC              "{"
  RCBRAC              "}"
  LSABRAC             "[<"
  RSABRAC             ">]"
  BACKSLASH           "\\"

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

%printer { yyo << $$; } <*>;

%%
%start program;

// Programs

program:
  PROGRAM IS body SEMICOLON { Logger::Debug("program"); }
;

body:
  decls BEGIN stmts END { Logger::Debug("body"); }
;

// Declarations

decls:
  %empty {}
| decls decl {}
;

decl:
  VAR var_decls {}
| PROCEDURE proc_decls {}
| TYPE type_decls {}
;

var_decls:
  %empty {}
| var_decls var_decl {}
;

var_decl:
  ids type_annot ASSIGN expr SEMICOLON { Logger::Debug("var-decl"); }
;

type_decls:
  %empty {}
| type_decls type_decl {}
;

type_decl:
  ID IS type SEMICOLON { Logger::Debug("type-decl"); }
;

proc_decls:
  %empty {}
| proc_decls proc_decl {}
;

proc_decl:
  ID LPAREN formal_params RPAREN type_annot IS body SEMICOLON { Logger::Debug("proc-decl"); }
;

formal_params:
  %empty {}
| formal_param {}
| formal_params SEMICOLON formal_param {}
;

formal_param:
  ids COLON type { Logger::Debug("formal-param"); }
;

type_annot:
  %empty {}
| COLON type { Logger::Debug("type-annot"); }
;

type:
  ID { Logger::Debug("id-type"); }
| ARRAY OF type[el_type] { Logger::Debug("array-type"); }
| RECORD components END { Logger::Debug("record-type"); }
;

components:
  component {}
| components component {}
;

component:
  ID COLON type SEMICOLON { Logger::Debug("component"); }
;

ids:
  ID {}
| ids COMMA ID {}
;

// Statements

stmts:
  %empty {}
| stmts stmt {}
;

stmt:
  lvalue ASSIGN expr SEMICOLON { Logger::Debug("assign-stmt"); }
| ID LPAREN actual_params RPAREN SEMICOLON { Logger::Debug("proc-call-stmt"); }
| READ LPAREN read_params RPAREN SEMICOLON { Logger::Debug("read-stmt"); }
| WRITE LPAREN write_params RPAREN SEMICOLON { Logger::Debug("write-stmt"); }
| IF expr THEN stmts elif_sections else_section END SEMICOLON { Logger::Debug("if-stmt"); }
| WHILE expr DO stmts END SEMICOLON { Logger::Debug("while-stmt"); }
| LOOP stmts END SEMICOLON { Logger::Debug("loop-stmt"); }
| FOR ID ASSIGN expr[begin] TO expr[end] for_step DO stmts END SEMICOLON { Logger::Debug("for-stmt"); }
| EXIT SEMICOLON { Logger::Debug("exit-stmt"); }
| RETURN SEMICOLON { Logger::Debug("return-stmt"); }
| RETURN expr SEMICOLON { Logger::Debug("return-stmt"); }
;

actual_params:
  %empty {}
| exprs { Logger::Debug("actual-params"); }
;

read_params:
  lvalues { Logger::Debug("read-params"); }
;

write_params:
  %empty {}
| write_exprs { Logger::Debug("write-params"); }
;

write_exprs:
  write_expr {}
| write_exprs COMMA write_expr {}
;

write_expr:
  STRING {}
| expr {}
;

elif_sections:
  %empty {}
| elif_sections elif_section {}
;

elif_section:
  ELSIF expr THEN stmts { Logger::Debug("elif-section"); }
;

else_section:
  %empty {}
| ELSE stmts { Logger::Debug("else-section"); }
;

for_step:
  %empty {}
| BY expr { Logger::Debug("for-step"); }
;

// Expressions

exprs:
  expr {}
| exprs COMMA expr {}
;

expr:
  number { Logger::Debug("number-expr"); }
| lvalue { Logger::Debug("lvalue-expr"); }
| LPAREN expr RPAREN { Logger::Debug("paren-expr"); }
| PLUS expr %prec POS { Logger::Debug("unary-expr"); }
| MINUS expr %prec NEG { Logger::Debug("unary-expr"); }
| NOT expr { Logger::Debug("unary-expr"); }
| expr PLUS expr { Logger::Debug("binary-expr"); }
| expr MINUS expr { Logger::Debug("binary-expr"); }
| expr STAR expr { Logger::Debug("binary-expr"); }
| expr SLASH expr { Logger::Debug("binary-expr"); }
| expr DIV expr { Logger::Debug("binary-expr"); }
| expr MOD expr { Logger::Debug("binary-expr"); }
| expr OR expr { Logger::Debug("binary-expr"); }
| expr AND expr { Logger::Debug("binary-expr"); }
| expr LT expr { Logger::Debug("binary-expr"); }
| expr LE expr { Logger::Debug("binary-expr"); }
| expr GT expr { Logger::Debug("binary-expr"); }
| expr GE expr { Logger::Debug("binary-expr"); }
| expr EQ expr { Logger::Debug("binary-expr"); }
| expr NE expr { Logger::Debug("binary-expr"); }
| ID LPAREN actual_params RPAREN { Logger::Debug("proc-call-expr"); }
| ID LCBRAC comp_values RCBRAC { Logger::Debug("record-construct-expr"); }
| ID LSABRAC array_values RSABRAC { Logger::Debug("array-construct-expr"); }
;

number:
  INTEGER {}
| REAL {}
;

lvalues:
  lvalue {}
| lvalues COMMA lvalue {}
;

lvalue:
  ID {}
| lvalue LSBRAC expr RSBRAC {}
| lvalue DOT ID {}
;

comp_values:
  assign_exprs { Logger::Debug("comp-values"); }
;

assign_exprs:
  assign_expr {}
| assign_exprs SEMICOLON assign_expr {}
;

assign_expr:
  ID ASSIGN expr {}
;

array_values:
  array_exprs { Logger::Debug("array-values"); }
;

array_exprs:
  array_expr {}
| array_exprs COMMA array_expr {}
;

array_expr:
  expr {}
| expr[n] OF expr[v] {}
;
%%

void yy::Parser::error(const location_type& loc, const std::string& msg) {
  Logger::Error(msg, &loc);
}
