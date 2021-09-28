#include <FlexLexer.h>

#include <fstream>
#include <iostream>
#include <memory>

#include "lexer.hpp"

int main(int argc, char** argv) {
  auto yyin = &std::cin;
  if (argc > 1) {
    yyin = new std::ifstream{argv[1]};
  }
  auto lexer = std::make_unique<yyFlexLexer>(yyin);

  int c;
  while ((c = lexer->yylex()) != T_EOF) {
    std::cout << "Scanner: " << lexer->YYText() << "\n";
  }

  if (yyin && yyin != &std::cin) {
    reinterpret_cast<std::ifstream*>(yyin)->close();
    delete yyin;
  }
}
