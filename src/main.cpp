#ifndef yyFlexLexerOnce
#include <FlexLexer.h>
#endif

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>

#include "config.hpp"
#include "io_buffer.hpp"
#include "lexer.hpp"

namespace fs = std::filesystem;

static size_t token_count = 0;
static size_t error_count = 0;

void PrintColumnHeadings(std::ostream& output) {
  output << std::left;
  output << std::setw(6) << "ROW";
  output << std::setw(6) << "COL";
  output << std::setw(20) << "TYPE";
  output << "TOKEN / ERROR MESSAGE\n";
  output << std::string(80, '-') << "\n";
}

void PrintStatistics(std::ostream& output) {
  output << std::string(80, '-') << "\n";
  output << "\nTotal: ";
  output << token_count << " tokens, ";
  output << error_count << " errors\n";
}

std::tuple<int, int> UpdatePosition(const yyFlexLexer& lexer) {
  static int cur_row = 1;     // self-maintained current row number
  static int cur_column = 1;  // self-maintained current column number

  auto start_row = cur_row;
  auto start_column = cur_column;
  std::string token = lexer.YYText();
  auto size = lexer.YYLeng();

  if (lexer.lineno() == start_row) {
    cur_column += size;
  } else {
    cur_row = lexer.lineno();
    auto endl_pos = token.find_last_of('\n');
    cur_column = endl_pos == std::string::npos ? size : size - endl_pos;
  }

  return {start_row, start_column};
}

// NOLINTNEXTLINE(runtime/references)
int ReadToken(yyFlexLexer& lexer, std::ostream& output) {
  static std::string buf;  // self-maintained buffer for comments
  static int start_row_bak = 1;
  static int start_column_bak = 1;

  auto t = lexer.yylex();
  auto [start_row, start_column] = UpdatePosition(lexer);
  std::string token = lexer.YYText();

  switch (t) {
    case T_EOF:
    case T_WS:
    case T_NEWLINE: break;  // skip whitespaces

    case T_COMMENTS_BEGIN: {
      start_row_bak = start_row;
      start_column_bak = start_column;
      // fallthrough
    }
    case T_COMMENTS: {
      buf += token;
      break;
    }
    case E_UNTERM_COMMENTS:
    case T_COMMENTS_END: {
      // Use the actual starting position of T_COMMENTS_BEGIN.
      start_row = start_row_bak;
      start_column = start_column_bak;
      token = buf + token;
      buf.clear();
      // fallthrough
    }

    default: {
      auto [type, error_msg] =
          [&token](int t) -> std::tuple<const char*, const char*> {
        switch (t) {
          case T_INTEGER: {
            if (token.size() > 10 || std::stoull(token) > INT32_MAX) {
              return {"error", "RangeError: out of range"};
            }
            return {"integer", NULL};
          }
          case T_REAL: return {"real", NULL};
          case T_STRING: {
            if (token.size() > 257) {
              return {"error", "ValueError: string literal is too long"};
            } else if (token.find('\t') != std::string::npos) {
              return {"error", "ValueError: invalid character found in string"};
            }
            return {"string", NULL};
          }
          case T_RESERVED: return {"reserved keyword", NULL};
          case T_IDENTIFIER: {
            if (token.size() > 255) {
              return {"error", "CompileError: identifier is too long"};
            }
            return {"identifier", NULL};
          }
          case T_OPERATOR: return {"operator", NULL};
          case T_DELIMITER: return {"delimiter", NULL};
          case T_COMMENTS_END: return {"comments", NULL};

          case E_UNTERM_STRING:
            return {"error", "SyntaxError: unterminated string literal"};
          case E_UNTERM_COMMENTS:
            return {"error", "SyntaxError: unterminated comments"};
          case E_UNKNOWN_CHAR:
            return {"error", "CompileError: unknown character"};
          default: return {"error", "UnknownError"};
        }
      }(t);

      output << std::setw(6) << start_row;
      output << std::setw(6) << start_column;
      output << std::setw(20) << type;
      if (error_msg) {
        output << error_msg << ": ";
        ++error_count;
      } else {
        ++token_count;
      }
      output << token << "\n";
    }
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
  auto& input = buf.yyin();
  auto& output = buf.yyout();

  auto p_lexer = std::make_unique<yyFlexLexer>(input, output);

  PrintColumnHeadings(output);
  while (true) {
    auto t = ReadToken(*p_lexer, buf.yyout());
    if (t == T_EOF || t == E_UNTERM_COMMENTS) {
      PrintStatistics(output);
      break;
    }
  }
}
