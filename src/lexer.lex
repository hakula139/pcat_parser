%{
#include "lexer.hpp"

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "driver.hpp"
#include "parser.hpp"

#define YY_USER_ACTION loc.step(); loc += YYLeng();

using location_type = yy::Parser::location_type;
using symbol_type = yy::Parser::symbol_type;
using MakeTable = std::unordered_map<
    std::string, symbol_type (*)(std::string s, location_type loc)>;

symbol_type make_INTEGER(const std::string& s, const location_type& loc);
symbol_type make_REAL(const std::string& s, const location_type& loc);
symbol_type make_STRING(const std::string& s, const location_type& loc);
void panic_UNTERM_STRING(const std::string& s, const location_type& loc);
symbol_type make_ID(const std::string& s, const location_type& loc);
symbol_type make_OPERATOR(const std::string& s, const location_type& loc);
symbol_type make_DELIMITER(const std::string& s, const location_type& loc);
void skip_COMMENTS(const std::string& s, const location_type& loc);
void panic_UNKNOWN_CHAR(const std::string& s, const location_type& loc);
%}

%option c++
%option noyywrap
%option nounput
%option noinput
%option batch
%option debug

%x      COMMENT

WS                    ([ \t\r]+)
NEWLINE               \n

DIGIT                 [0-9]
INTEGER               ({DIGIT}+)
REAL                  ({DIGIT}+"."{DIGIT}*)
LETTER                [A-Za-z]
STRING                (\"[^\n"]*\")
UNTERM_STRING         (\"[^\n"]*)

IDENTIFIER            ({LETTER}({LETTER}|{DIGIT})*)
OPERATOR              (":="|"+"|"-"|"*"|"/"|"<"|"<="|">"|">="|"="|"<>")
DELIMITER             (":"|";"|","|"."|"("|")"|"["|"]"|"{"|"}"|"[<"|">]"|"\\")

COMMENTS_BEGIN        "(*"
COMMENTS_END          "*)"

%%

%{
  auto& loc = drv_.loc();
%}

<INITIAL><<EOF>>              { return yy::Parser::make_YYEOF(loc); }
<INITIAL>{WS}                 /* skip whitespaces */
<INITIAL>{NEWLINE}            { loc.lines(); }

<INITIAL>{INTEGER}            { return make_INTEGER(YYText(), loc); }
<INITIAL>{REAL}               { return make_REAL(YYText(), loc); }
<INITIAL>{STRING}             { return make_STRING(YYText(), loc); }
<INITIAL>{UNTERM_STRING}      { panic_UNTERM_STRING(YYText(), loc); }

<INITIAL>{IDENTIFIER}         { return make_ID(YYText(), loc); }
<INITIAL>{OPERATOR}           { return make_OPERATOR(YYText(), loc); }
<INITIAL>{DELIMITER}          { return make_DELIMITER(YYText(), loc); }

<INITIAL>{COMMENTS_BEGIN}     { BEGIN(COMMENT); skip_COMMENTS(YYText(), loc); }
<COMMENT>{COMMENTS_END}       { BEGIN(INITIAL); skip_COMMENTS(YYText(), loc); }
<COMMENT>.                    { skip_COMMENTS(YYText(), loc); }
<COMMENT>{NEWLINE}            { loc.lines(); skip_COMMENTS(YYText(), loc); }
<COMMENT><<EOF>>              { BEGIN(INITIAL); skip_COMMENTS("", loc); }

<INITIAL>.                    { panic_UNKNOWN_CHAR(YYText(), loc); }

%%

symbol_type make_INTEGER(const std::string& s, const location_type& loc) {
  try {
    std::stoi(s);
  } catch (const std::out_of_range& e) {
    throw yy::Parser::syntax_error(loc, "range error, integer out of range: " + s);
  }
  return yy::Parser::make_INTEGER(s, loc);
}

symbol_type make_REAL(const std::string& s, const location_type& loc) {
  return yy::Parser::make_REAL(s, loc);
}

symbol_type make_STRING(const std::string& s, const location_type& loc) {
  if (s.size() > 257) {
    throw yy::Parser::syntax_error(
        loc, "value error, string literal is too long: " + s);
  }

  if (s.find('\t') != std::string::npos) {
    throw yy::Parser::syntax_error(
        loc, "value error, invalid character found in string: " + s);
  }

  return yy::Parser::make_STRING(s, loc);
}

void panic_UNTERM_STRING(const std::string& s, const location_type& loc) {
  throw yy::Parser::syntax_error(
      loc, "syntax error, unterminated string literal: " + s);
}

static const MakeTable make_keyword_table{
    {"AND", yy::Parser::make_AND},
    {"ARRAY", yy::Parser::make_ARRAY},
    {"BEGIN", yy::Parser::make_BEGIN},
    {"BY", yy::Parser::make_BY},
    {"DIV", yy::Parser::make_DIV},
    {"DO", yy::Parser::make_DO},
    {"ELSE", yy::Parser::make_ELSE},
    {"ELSIF", yy::Parser::make_ELSIF},
    {"END", yy::Parser::make_END},
    {"EXIT", yy::Parser::make_EXIT},
    {"FOR", yy::Parser::make_FOR},
    {"IF", yy::Parser::make_IF},
    {"IN", yy::Parser::make_IN},
    {"IS", yy::Parser::make_IS},
    {"LOOP", yy::Parser::make_LOOP},
    {"MOD", yy::Parser::make_MOD},
    {"NOT", yy::Parser::make_NOT},
    {"OF", yy::Parser::make_OF},
    {"OR", yy::Parser::make_OR},
    {"OUT", yy::Parser::make_OUT},
    {"PROCEDURE", yy::Parser::make_PROCEDURE},
    {"PROGRAM", yy::Parser::make_PROGRAM},
    {"READ", yy::Parser::make_READ},
    {"RECORD", yy::Parser::make_RECORD},
    {"RETURN", yy::Parser::make_RETURN},
    {"THEN", yy::Parser::make_THEN},
    {"TO", yy::Parser::make_TO},
    {"TYPE", yy::Parser::make_TYPE},
    {"VAR", yy::Parser::make_VAR},
    {"WHILE", yy::Parser::make_WHILE},
    {"WRITE", yy::Parser::make_WRITE},
};

symbol_type make_ID(const std::string& s, const location_type& loc) {
  if (s.size() > 255) {
    throw yy::Parser::syntax_error(
        loc, "compile error, identifier is too long: " + s);
  }

  auto entry = make_keyword_table.find(s);
  if (entry != make_keyword_table.end()) {
    return (entry->second)(s, loc);
  }

  return yy::Parser::make_ID(s, loc);
}

static const MakeTable make_operator_table{
    {":=", yy::Parser::make_ASSIGN},
    {"+", yy::Parser::make_PLUS},
    {"-", yy::Parser::make_MINUS},
    {"*", yy::Parser::make_STAR},
    {"/", yy::Parser::make_SLASH},
    {"<", yy::Parser::make_LT},
    {"<=", yy::Parser::make_LE},
    {">", yy::Parser::make_GT},
    {">=", yy::Parser::make_GE},
    {"=", yy::Parser::make_EQ},
    {"<>", yy::Parser::make_NE},
};

symbol_type make_OPERATOR(const std::string& s, const location_type& loc) {
  return make_operator_table.at(s)(s, loc);
}

static const MakeTable make_delimiter_table{
  {":", yy::Parser::make_COLON},
  {";", yy::Parser::make_SEMICOLON},
  {",", yy::Parser::make_COMMA},
  {".", yy::Parser::make_DOT},
  {"(", yy::Parser::make_LPAREN},
  {")", yy::Parser::make_RPAREN},
  {"[", yy::Parser::make_LSBRAC},
  {"]", yy::Parser::make_RSBRAC},
  {"{", yy::Parser::make_LCBRAC},
  {"}", yy::Parser::make_RCBRAC},
  {"[<", yy::Parser::make_LSABRAC},
  {">]", yy::Parser::make_RSABRAC},
  {"\\", yy::Parser::make_BACKSLASH},
};

symbol_type make_DELIMITER(const std::string& s, const location_type& loc) {
  return make_delimiter_table.at(s)(s, loc);
}

void skip_COMMENTS(const std::string& s, const location_type& loc) {
  static std::string comments_buf;
  static location_type comments_loc;

  if (s == "(*") {
    comments_buf = s;
    comments_loc = loc;
  } else if (!s.empty()) {
    comments_buf += s;
    comments_loc += loc;
  } else {
    throw yy::Parser::syntax_error(
        comments_loc, "syntax error, unterminated comments: " + comments_buf);
  }
}

void panic_UNKNOWN_CHAR(const std::string& s, const location_type& loc) {
  throw yy::Parser::syntax_error(loc, "compile error, unknown character: " + s);
}
