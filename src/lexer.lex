%{
#include "lexer.hpp"
%}

%option c++
%option noyywrap

WS                    ([ \t]+)
NEWLINE               \r?\n

DIGIT                 [0-9]
INTEGER               ({DIGIT}+)
REAL                  ({DIGIT}+"."{DIGIT}*)
LETTER                [A-Za-z]
STRING                (\"[^\t\n"]+\")

RESERVED              (AND|ARRAY|BEGIN|BY|DIV|DO|ELSE|ELSIF|END|EXIT|FOR|IF|IN|IS|LOOP|MOD|NOT|OF|OR|OUT|PROCEDURE|PROGRAM|READ|RECORD|RETURN|THEN|TO|TYPE|VAR|WHILE|WRITE)
IDENTIFIER            {LETTER}({LETTER}|{DIGIT})*
OPERATOR              (":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>")
DELIMITER             (":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|"\\")

COMMENTS              ("(*".*"*)")

%%
{WS}                  /* skip whitespaces */
{NEWLINE}             { ++yylineno; }
<<EOF>>               return T_EOF;

{INTEGER}             return T_INTEGER;
{REAL}                return T_REAL;
{STRING}              return T_STRING;

{RESERVED}            return T_RESERVED;
{IDENTIFIER}          return T_IDENTIFIER;
{OPERATOR}            return T_OPERATOR;
{DELIMITER}           return T_DELIMITER;

{COMMENTS}            return T_COMMENTS;
%%
