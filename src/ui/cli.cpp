#include "ui/cli.hpp"
#include "core/DatabaseManager.hpp"

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
    } else if (arg == "-u" || arg == "--user") {
      // TODO: check that i+1 is correctly formated
      options.hasUser = true;
      if (argc <= i) {
        std::cerr << "Warning: please provide a Username\n";
      }
      options.username = argv[i + 1];
      ++i;
    } else if (arg == "-l" || arg == "--list") {
      options.listMovies = true;
    } else if (arg == "-m" || arg == "--movie") {
      options.addMovie = true;
      if (argc <= i) {
        std::cerr << "Warning: please provide a Username\n";
      }
      options.movieName = argv[i + 1];
      ++i;
      std::string_view tmp(argv[i + 1]);
      if (tmp == "-y" || tmp == "--year") {
        i++;
        if (argc <= i) {
          std::cerr << "Warning: please provide a Rating\n";
        }
        options.movieYear = atoi(argv[i + 1]);
      }

    } else {
      // You can handle unknown flags or positional arguments here
      std::cerr << "Warning: Unknown argument '" << arg << "'\n";
    }
  }

  // Pass the newly populated options struct to the private constructor
  return CLI(std::move(options), "data.db3");
}

void CLI::execOptions() {
  // flags
  if (m_options.showHelp) {
    CLI::showHelp();
    return;
  }
  if (m_options.showVersion) {
    CLI::showVersion();
    return;
  }
  if (m_options.listMovies) {
    // CLI::listMovies();
  }

  // keyword arguments
  if (m_options.hasUser) {
    CLI::addUser();
  }

  if (m_options.addMovie) {
    CLI::addMovie();
  }
}

void CLI::addUser() { m_db.setUser(m_options.username); }
void CLI::addMovie() {
  CLI::m_db.addMovie(m_options.movieName, core::MOVIE, m_options.movieYear);
}

void CLI::showHelp() { std::cout << "HELPSCREEN\n"; }

void CLI::showVersion() { std::cout << "VERSIONSCREEN\n"; }

} // namespace ui
