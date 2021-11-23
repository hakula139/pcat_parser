#include "utils.hpp"

#include <iomanip>
#include <iostream>
#include <string>

void PrintColumnHeadings(std::ostream& output) {
  output << std::left << std::setw(6) << "ROW";
  output << std::setw(6) << "COL";
  output << std::setw(20) << "TYPE";
  output << "TOKEN / ERROR MESSAGE\n";
  output << std::string(80, '-') << "\n";
}

void PrintStatistics(
    std::ostream& output, size_t token_count, size_t error_count) {
  output << std::string(80, '-') << "\n";
  output << "\nTotal: ";
  output << token_count << " tokens, ";
  output << error_count << " errors\n";
}
