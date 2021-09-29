#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <tuple>

#include "config.hpp"
#include "io_buffer.hpp"
#include "lexer.hpp"

namespace fs = std::filesystem;

void PrintColumnHeadings(std::ostream& output) {
  output << std::left;
  output << std::setw(6) << "ROW";
  output << std::setw(6) << "COL";
  output << std::setw(20) << "TYPE";
  output << "TOKEN / ERROR MESSAGE\n";
  output << std::string(80, '-') << "\n";
}

int UpdateColumn(const yyFlexLexer& lexer) {
  static int yycolumn = 1;  // self-maintained current column number
  static int start_row = 1;
  auto start_column = yycolumn;

  auto token = lexer.YYText();
  auto size = lexer.YYLeng();
  if (lexer.lineno() == start_row) {
    yycolumn += size;
  } else {
    for (yycolumn = 1; size >= yycolumn && token[size - yycolumn] != '\n';
         ++yycolumn) {
    }
    start_row = lexer.lineno();
  }

  return start_column;
}

// NOLINTNEXTLINE(runtime/references)
std::tuple<int, size_t> ReadToken(yyFlexLexer& lexer, std::ostream& output) {
  static size_t token_count = 0;

  auto t = lexer.yylex();
  auto start_row = lexer.lineno();
  auto start_column = UpdateColumn(lexer);

  if (t != T_EOF && t != T_WS && t != T_NEWLINE) {
    auto [type, error_msg] = [](int t) -> std::tuple<const char*, const char*> {
      switch (t) {
        case T_EOF: return {"eof", NULL};
        case T_INTEGER: return {"integer", NULL};
        case T_REAL: return {"real", NULL};
        case T_STRING: return {"string", NULL};
        case T_RESERVED: return {"reserved keyword", NULL};
        case T_IDENTIFIER: return {"identifier", NULL};
        case T_OPERATOR: return {"operator", NULL};
        case T_DELIMITER: return {"delimiter", NULL};
        case T_COMMENTS: return {"comments", NULL};

        case E_UNTERM_STRING:
          return {"error", "SyntaxError: unterminated string literal"};
        default: return {"error", "unknown error"};
      }
    }(t);

    auto token = lexer.YYText();

    output << std::setw(6) << start_row;
    output << std::setw(6) << start_column;
    output << std::setw(20) << type;
    if (error_msg) {
      output << error_msg << ": ";
    }
    output << token << "\n";

    ++token_count;
  }

  return {t, token_count};
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
  auto& input = buf.yyin();
  auto& output = buf.yyout();

  auto p_lexer = std::make_unique<yyFlexLexer>(input, output);

  PrintColumnHeadings(output);
  while (true) {
    auto [t, token_count] = ReadToken(*p_lexer, buf.yyout());
    if (t == T_EOF) {
      output << "\nTotal: " << token_count << " tokens\n";
      break;
    }
  }
}
