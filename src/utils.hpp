#ifndef SRC_UTILS_HPP_
#define SRC_UTILS_HPP_

#include <iostream>

void PrintColumnHeadings(std::ostream& output);

void PrintStatistics(
    std::ostream& output, size_t token_count, size_t error_count);

#endif  // SRC_UTILS_HPP_
