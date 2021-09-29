#ifndef SRC_IO_BUFFER_HPP_
#define SRC_IO_BUFFER_HPP_

#include <fstream>
#include <iostream>

class IOBuffer {
 public:
  explicit IOBuffer(
      const char* input_path = nullptr, const char* output_path = nullptr);
  ~IOBuffer();

  std::istream& yyin() { return *yyin_; }
  std::ostream& yyout() { return *yyout_; }

 protected:
  std::istream* const yyin_ = &std::cin;
  std::ostream* const yyout_ = &std::cout;
};

#endif  // SRC_IO_BUFFER_HPP_
