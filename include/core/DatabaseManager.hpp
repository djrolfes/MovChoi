#pragma once
#include "core/User.hpp"
#include "core/utils.hpp"
#include <cstring>
#include <fstream>
#include <sqlite3.h>
#include <sstream>
#include <string>
namespace core {
enum MovieType { MOVIE, SERIES };
int exec_sql_from_file(sqlite3 *db, const std::string &filename);

class DatabaseManager {
public:
  DatabaseManager(std::string dbname = "data.db3");
  ~DatabaseManager();
  int add_user(const std::string &username);
  int get_user(const std::string &username, User &user);
  int add_movie(const std::string &title, const MovieType type = MOVIE,
                const int release_year = -1);
  int add_watchlist_entry(const int user_id, const int movie_id,
                          const std::string date = today(),
                          const int rating = -1);
  // int add_watchlist_entry(const int user_id, const std::string &title,
  //                         const std::string date = today(),
  //                         const int rating = -1);

private:
  const std::string m_dbname;
  sqlite3 *m_db;
  User user;
};
} // namespace core
