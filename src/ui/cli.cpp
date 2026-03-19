#include "ui/cli.hpp"

namespace ui {

CLI CLI::fromArgs(int argc, char *argv[]) {
  CLI::Options options;

  // We start the loop at index 1.
  // index 0 is always the name/path of the program itself (e.g., "./my_app")
  for (int i = 1; i < argc; ++i) {
    // std::string_view provides fast, zero-copy string comparisons
    std::string_view arg(argv[i]);

    if (arg == "-h" || arg == "--help") {
      options.showHelp = true;
    } else if (arg == "-v" || arg == "--version") {
      options.showVersion = true;
    } else {
      // You can handle unknown flags or positional arguments here
      std::cerr << "Warning: Unknown argument '" << arg << "'\n";
    }
  }

  // Pass the newly populated options struct to the private constructor
  return CLI(std::move(options), "data.db3");
}

} // namespace ui
