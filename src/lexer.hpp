#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_

enum Tokens {
  T_EOF = 0,
  T_WS,
  T_NEWLINE,
  T_INTEGER,
  T_REAL,
  T_STRING,
  T_RESERVED,
  T_IDENTIFIER,
  T_OPERATOR,
  T_DELIMITER,
  T_COMMENTS,
};

enum Errors {
  E_UNTERM_STRING = 10000,
};

#endif  // SRC_LEXER_HPP_
