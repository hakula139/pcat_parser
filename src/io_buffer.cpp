#include "io_buffer.hpp"

#include <fstream>
#include <iostream>

IOBuffer::IOBuffer(const char* input_path, const char* output_path)
    : p_input_{input_path ? new std::ifstream{input_path} : &std::cin},
      p_output_{output_path ? new std::ofstream{output_path} : &std::cout} {}

IOBuffer::~IOBuffer() {
  if (p_input_ && p_input_ != &std::cin) {
    reinterpret_cast<std::ifstream*>(p_input_)->close();
    delete p_input_;
  }
  if (p_output_ && p_output_ != &std::cout) {
    reinterpret_cast<std::ofstream*>(p_output_)->close();
    delete p_output_;
  }
}
