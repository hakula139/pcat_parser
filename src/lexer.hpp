#ifndef SRC_LEXER_HPP_
#define SRC_LEXER_HPP_

#include <fstream>

#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include "parser.hpp"

// Override the default yylex() to return a symbol_type instead of int.
#define YY_DECL yy::Parser::symbol_type yy::Lexer::ReadToken()

class Driver;

namespace yy {
class Lexer : public yyFlexLexer {
 public:
  explicit Lexer(Driver* p_drv) : drv_{*p_drv} {}

  virtual yy::Parser::symbol_type ReadToken();

 protected:
  Driver& drv_;
};
}  // namespace yy

#endif  // SRC_LEXER_HPP_
