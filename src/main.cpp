#include <FlexLexer.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "config.hpp"
#include "io_buffer.hpp"
#include "lexer.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  auto buf = [argc, argv]() {
    if (argc == 1) return IOBuffer{};

    fs::path input_path = argv[1];
    std::cout << "Input path: " << input_path.c_str() << "\n";
    fs::path output_path = output_dir /
                           (input_path.filename().replace_extension(".txt"));
    std::cout << "Output path: " << output_path.c_str() << "\n";
    return IOBuffer{input_path.c_str(), output_path.c_str()};
  }();

  auto lexer = std::make_unique<yyFlexLexer>(buf.yyin(), buf.yyout());

  int c = T_EOF;
  while ((c = lexer->yylex()) != T_EOF) {
    buf.yyout() << lexer->lineno() << ": " << lexer->YYText() << "\n";
  }
}
