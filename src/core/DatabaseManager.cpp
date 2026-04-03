#include "core/DatabaseManager.hpp"
#include "SQLiteCpp/Database.h"
#include "SQLiteCpp/Statement.h"
#include <string>
#include <vector>

namespace core {

int DatabaseManager::getColumnString(const std::string &columnName,
                                     const std::string &tableName,
                                     std::vector<std::string> &out) {
  // TODO: cache of tableNames and corresponding columnNames for whitelist
  SQLite::Statement query(mDb,
                          "SELECT " + columnName + " From " + tableName + ";");
  while (query.executeStep()) {
    out.push_back((std::string)query.getColumn(0));
  }
  return 0;
}

DatabaseManager::~DatabaseManager() {}

int DatabaseManager::getUser(const std::string &username, User &user) {
  SQLite::Statement query(mDb, "SELECT id FROM Users WHERE username = ?;");
  query.bind(0, username.c_str());
  int rc = query.executeStep();

  if (rc == SQLITE_ROW) {
    user.id = query.getColumn("id");
    user.name = username;
  } else if (rc == SQLITE_DONE) {
    // No user found
    user.id = -1;
    user.name = "";
  } else {
    return 1;
  }
  return 0;
}

std::vector<std::string> DatabaseManager::getUsernames() {
  std::vector<std::string> rtn;
  int rc = getColumnString("username", "users", rtn);
  // TODO:: handle r!=0
  return rtn;
}

std::vector<std::vector<std::string>> DatabaseManager::getUsernamesVertical() {
  std::vector<std::string> tmp(getUsernames());
  std::vector<std::vector<std::string>> rtn;
  for (int i = 0; i < tmp.size(); i++) {
    rtn.push_back({tmp[i]});
  }
  return rtn;
}

int DatabaseManager::addUser(const std::string &username) {
  if (userExists(username)) {
    // TODO: add call to allow for overwrite of existing user
    fprintf(stderr, "User already exists: %s\n", username.c_str());
    return 1;
  }

  SQLite::Statement query(mDb, "INSERT INTO Users (username) VALUES (?);");
  query.bind(0, username.c_str());
  int rc = query.exec();

  rc = getUser(username, user);
  if (user.id == -1) {
    fprintf(stderr, "User could not be added: %s\n", username.c_str());
    return 1;
  }
  if (rc != 0) {
    fprintf(stderr, "Error checking for existing user: %s\n", username.c_str());
    return 1;
  }

  return 0;
}

int DatabaseManager::addMovie(const std::string &title, const MovieType type,
                              const int release_year) {
  std::string sql;
  if (release_year != -1) {
    sql = "INSERT OR IGNORE INTO movies (title, type, release_year) VALUES "
          "(?,?,?);";
  } else {
    sql = "INSERT OR IGNORE INTO movies (title, type) VALUES (?,?);";
  }

  SQLite::Statement query(mDb, sql);

  query.bind(1, title);
  query.bind(2, type);

  if (release_year != -1) {
    query.bind(3, release_year);
  }
  query.exec();

  return 0;
}

int DatabaseManager::addWatchlistEntry(const int user_id, const int movie_id,
                                       std::string date, const int rating) {

  std::string sql;
  if (rating != -1) {
    sql = "INSERT OR IGNORE INTO watchlist (user_id, movie_id, "
          "date) VALUES (?,?,?);";
  } else {
    sql = "INSERT OR IGNORE INTO watchlist VALUES (?,?,?,?);";
  }

  SQLite::Statement query(mDb, sql);

  query.bind(1, user_id);
  query.bind(2, movie_id);
  query.bind(3, date);

  if (rating != -1) {
    query.bind(4, rating);
  }
  query.exec();

  return 0;
}

bool DatabaseManager::userExists(const std::string &username) {
  SQLite::Statement query(mDb, "SELECT (id) from users where username = (?);");
  query.bind(1, username);
  return query.executeStep();
}

int DatabaseManager::setUser(const User &user) {
  if (!userExists(user.name)) {
    addUser(user.name);
  }
  DatabaseManager::user = user;
  return 0;
}

int DatabaseManager::setUser(const std::string &username) {
  if (!userExists(username)) {
    addUser(username);
  }
  getUser(username, user);
  return 0;
}

int DatabaseManager::removeUser(const std::string &username) {

  if (!DatabaseManager::userExists(username)) {
    fprintf(stderr, "User does not exists: %s\n", username.c_str());
    return 1;
  }

  SQLite::Statement query(mDb, "DELETE FROM Users WHERE username=(?);");
  query.bind(1, username);
  query.exec();
  return 0;
}
// int DatabaseManager::add_watchlist_entry(const int user_id,
//                                          const std::string &title,
//                                          const std::string date,
//                                          const int rating) {
//
//   return 0;
// }

int exec_sql_from_file(SQLite::Database &db, const std::string &filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    fprintf(stderr, "Could not open file: %s\n", filename.c_str());
    return -1;
  }

  // 2. Read the entire file into a std::string buffer
  std::stringstream buffer;
  buffer << file.rdbuf();
  std::string sql = buffer.str();

  // 3. Execute the SQL
  int rtn = db.exec(sql);

  return rtn;
}

} // namespace core
