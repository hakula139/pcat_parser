%skeleton "lalr1.cc"
%require "3.5"
%header "parser.hpp"
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

%code requires {
  #include <string>

  class Driver;
}

%code top {
  #include "Driver.hpp"
}

%param {Driver& drv}

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

  <uint64_t>          INTEGER             "integer"
  <double>            REAL                "real"
  <std::string>       IDENTIFIER          "identifier"
;

%printer { yyo << $$; } <>;

%%
%start program;

program:
  PROGRAM IS body ";" {
    drv.program = $body;
  }
;

body:
  decls BEGIN stmts END {
  }
;
%%
