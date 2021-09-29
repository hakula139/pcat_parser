#include "io_buffer.hpp"

#include <fstream>
#include <iostream>

IOBuffer::IOBuffer(const char* input_path, const char* output_path)
    : yyin_{input_path ? new std::ifstream{input_path} : &std::cin},
      yyout_{output_path ? new std::ofstream{output_path} : &std::cout} {}

IOBuffer::~IOBuffer() {
  if (yyin_ && yyin_ != &std::cin) {
    reinterpret_cast<std::ifstream*>(yyin_)->close();
    delete yyin_;
  }
  if (yyout_ && yyout_ != &std::cout) {
    reinterpret_cast<std::ofstream*>(yyout_)->close();
    delete yyout_;
  }
}
