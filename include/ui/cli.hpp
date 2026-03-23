#pragma once
#include "core/DatabaseManager.hpp"
#include <iostream>
#include <string_view>

namespace ui {
class CLI {
  struct Options {
    bool showHelp = false;
    bool showVersion = false;
    bool hasUser = false;
    std::string username;
    bool listMovies = false;
    bool addMovie = false;
    std::string movieName;
    int movieYear = -1;
  };

public:
  static CLI fromArgs(int argc, char *argv[]);

private:
  void execOptions();
  void showHelp();
  void showVersion();
  void addUser();
  void listMovies();
  void addMovie();

  CLI(Options options, std::string dbname)
      : m_options(std::move(options)), m_db(dbname) {
    execOptions();
  }
  Options m_options;
  core::DatabaseManager m_db;
};
} // namespace ui
