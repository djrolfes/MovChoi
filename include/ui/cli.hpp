#pragma once
#include "core/DatabaseManager.hpp"
#include <iostream>
#include <string_view>

namespace ui {
class CLI {
  struct Options {
    bool showHelp = false;
    bool showVersion = false;
  };

public:
  static CLI fromArgs(int argc, char *argv[]);

private:
  CLI(Options options, std::string dbname)
      : m_options(std::move(options)), m_db(dbname) {}
  Options m_options;
  core::DatabaseManager m_db;
};
} // namespace ui
