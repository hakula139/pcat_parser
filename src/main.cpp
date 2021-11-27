#include <filesystem>  // fs::path

#include "base/config.hpp"
#include "driver.hpp"
#include "utils/io_buffer.hpp"
#include "utils/logger.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  // For self-destruction when exceptions occurred.
  auto buf = [argc, argv]() {
    if (argc == 1) return IOBuffer{};

    const auto IN_PATH = fs::path{argv[1]};
    Logger::Info("input path: " + IN_PATH.string());

    const auto OUT_FILENAME = IN_PATH.filename().replace_extension(".txt");
    const auto OUT_PATH = OUTPUT_DIR / OUT_FILENAME;
    Logger::Info("output path: " + OUT_PATH.string());

    return IOBuffer{IN_PATH.c_str(), OUT_PATH.c_str()};
  }();

  Driver drv;
  int ret = drv.Parse(buf.input(), buf.output());
  Logger::Info("parse complete, return code: " + std::to_string(ret));
  return ret;
}
