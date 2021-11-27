#include <filesystem>  // std::filesystem::path

#include "base/config.hpp"
#include "driver.hpp"
#include "utils/logger.hpp"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
  if (argc == 1) {
    Logger::Fatal("no input path specified");
    return EXIT_FAILURE;
  }

  const auto in_path = fs::path{argv[1]};
  Logger::Info("input path: " + in_path.string());

  const auto out_file = in_path.filename().replace_extension(".txt");
  const auto out_path = OUTPUT_DIR / out_file;
  Logger::Info("output path: " + out_path.string());

  Logger::Info("parse started");
  Driver drv;
  int ret = drv.Parse(in_path, out_path);
  Logger::Info("parse complete, return code: " + std::to_string(ret));
  return ret;
}
