#include <FlexLexer.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "config.hpp"
#include "io_buffer.hpp"
#include "lexer.hpp"

namespace fs = std::filesystem;

void PrintColumnHeadings(std::ostream& output) {
  output << std::left;
  output << std::setw(6) << "ROW";
  output << std::setw(20) << "TYPE";
  output << "TOKEN\n";
  output << std::string(80, '-') << "\n";
}

// NOLINTNEXTLINE(runtime/references)
int ReadToken(yyFlexLexer& lexer, std::ostream& output) {
  auto t = lexer.yylex();
  if (t != T_EOF) {
    auto row = lexer.lineno();
    auto type = [](int t) {
      switch (t) {
        case T_EOF: return "eof";
        case T_INTEGER: return "integer";
        case T_REAL: return "real";
        case T_STRING: return "string";
        case T_RESERVED: return "reserved keyword";
        case T_IDENTIFIER: return "identifier";
        case T_OPERATOR: return "operator";
        case T_DELIMITER: return "delimiter";
        case T_COMMENTS: return "comments";
        default: return "unknown";
      }
    }(t);
    auto token = lexer.YYText();

    output << std::setw(6) << row;
    output << std::setw(20) << type;
    output << token << "\n";
  }
  return t;
}

int main(int argc, char** argv) {
  auto buf = [argc, argv]() {
    if (argc == 1) return IOBuffer{};

    auto input_path = fs::path{argv[1]};
    std::cout << "Input path: " << input_path.c_str() << "\n";

    auto output_filename = input_path.filename().replace_extension(".txt");
    auto output_path = output_dir / output_filename;
    std::cout << "Output path: " << output_path.c_str() << "\n";

    return IOBuffer{input_path.c_str(), output_path.c_str()};
  }();

  auto p_lexer = std::make_unique<yyFlexLexer>(buf.yyin(), buf.yyout());
  PrintColumnHeadings(buf.yyout());
  while (ReadToken(*p_lexer, buf.yyout()) != T_EOF) {
  }
}
