%{
#include "lexer.hpp"
%}

%option c++
%option noyywrap

WS                    [ \t]+
DIGIT                 [0-9]
INTEGER               {DIGIT}+
REAL                  {DIGIT}+\.{DIGIT}*
ALPHA                 [A-Za-z]
STRING                \"[^\n"]+\"

%%
{WS}                  /* skip whitespaces */
<<EOF>>               return T_EOF;
{INTEGER}|{REAL}      return NUMBER;
"+"                   return ADD;
"-"                   return SUB;
%%
