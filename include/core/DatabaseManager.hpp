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
  int addUser(const std::string &username);
  int getUser(const std::string &username, User &user);
  int removeUser(const std::string &username);
  bool userExists(const std::string &username);
  int setUser(const User &user);
  int setUser(const std::string &username);
  std::vector<std::string> getUsernames();
  std::vector<std::vector<std::string>> getUsernamesVertical();
  int addMovie(const std::string &title, const MovieType type = MOVIE,
               const int release_year = -1);
  int addWatchlistEntry(const int user_id, const int movie_id,
                        const std::string date = today(),
                        const int rating = -1);

  // int add_watchlist_entry(const int user_id, const std::string &title,
  //                         const std::string date = today(),
  //                         const int rating = -1);

private:
  int getColumnString(const std::string &columnName,
                      const std::string &tableName,
                      std::vector<std::string> &out);
  const std::string m_dbname;
  sqlite3 *m_db;
  User user;
};
} // namespace core
