#ifndef SRC_BASE_COMMON_HPP_
#define SRC_BASE_COMMON_HPP_

#include <memory>  // std::unique_ptr, std::shared_ptr

// ANSI colors
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define YELLOW "\e[0;33m"
#define BLUE "\e[0;34m"
#define PURPLE "\e[0;35m"
#define CYAN "\e[0;36m"
#define RESET "\e[0;0m"

enum LogLevel {
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

template <class T>
using UPtr = typename std::unique_ptr<T>;

template <class T>
using SPtr = typename std::shared_ptr<T>;

// Code snippets for visiting std::variant.
// See: https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct Overloaded : Ts... {
  using Ts::operator()...;
};
template <class... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

#endif  // SRC_BASE_COMMON_HPP_
