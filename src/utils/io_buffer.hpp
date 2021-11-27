#ifndef SRC_UTILS_IO_BUFFER_HPP_
#define SRC_UTILS_IO_BUFFER_HPP_

#include <fstream>
#include <iostream>

class IOBuffer {
 public:
  IOBuffer() {}
  IOBuffer(const char* input_path, const char* output_path);
  ~IOBuffer();

  std::istream& input() const { return *p_input_; }
  std::ostream& output() const { return *p_output_; }

 protected:
  std::istream* const p_input_ = &std::cin;
  std::ostream* const p_output_ = &std::cout;
};

#endif  // SRC_UTILS_IO_BUFFER_HPP_
