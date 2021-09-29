#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>

#include "io_buffer.hpp"
#include "lexer.hpp"

constexpr const char* output_path = "output/result.txt";

int main(int argc, char** argv) {
  const char* input_path = argc > 1 ? argv[1] : nullptr;
  IOBuffer buf{input_path, output_path};

  auto lexer = std::make_unique<yyFlexLexer>(buf.yyin(), buf.yyout());

  int c = T_EOF;
  while ((c = lexer->yylex()) != T_EOF) {
    buf.yyout() << "<" << lexer->YYText() << ">";
  }
}
