%{
#include "lexer.hpp"
%}

%option c++
%option noyywrap
%option yylineno

%x      COMMENT

WS                    ([ \t]+)
NEWLINE               (\r?\n)

DIGIT                 [0-9]
INTEGER               ({DIGIT}+)
REAL                  ({DIGIT}+"."{DIGIT}*)
LETTER                [A-Za-z]
STRING                (\"[^\n"]*\")
UNTERM_STRING         (\"[^\n"]*)

RESERVED              (AND|ARRAY|BEGIN|BY|DIV|DO|ELSE|ELSIF|END|EXIT|FOR|IF|IN|IS|LOOP|MOD|NOT|OF|OR|OUT|PROCEDURE|PROGRAM|READ|RECORD|RETURN|THEN|TO|TYPE|VAR|WHILE|WRITE)
IDENTIFIER            {LETTER}({LETTER}|{DIGIT})*
OPERATOR              (":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>")
DELIMITER             (":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|"\\")

COMMENTS_BEGIN        "(*"
COMMENTS_END          "*)"

%%
<INITIAL><<EOF>>              return T_EOF;
<INITIAL>{WS}                 return T_WS;
<INITIAL>{NEWLINE}            return T_NEWLINE;

<INITIAL>{INTEGER}            return T_INTEGER;
<INITIAL>{REAL}               return T_REAL;
<INITIAL>{STRING}             return T_STRING;
<INITIAL>{UNTERM_STRING}      return E_UNTERM_STRING;

<INITIAL>{RESERVED}           return T_RESERVED;
<INITIAL>{IDENTIFIER}         return T_IDENTIFIER;
<INITIAL>{OPERATOR}           return T_OPERATOR;
<INITIAL>{DELIMITER}          return T_DELIMITER;

<INITIAL>{COMMENTS_BEGIN}     BEGIN(COMMENT); return T_COMMENTS_BEGIN;
<COMMENT>{COMMENTS_END}       BEGIN(INITIAL); return T_COMMENTS_END;
<COMMENT>(.|{NEWLINE})        return T_COMMENTS;
<COMMENT><<EOF>>              return E_UNTERM_COMMENTS;

<INITIAL>.                    return E_UNKNOWN_CHAR;
%%
