#include "core/DatabaseManager.hpp"

namespace core {
DatabaseManager::DatabaseManager(std::string dbname)
    : m_dbname(std::move(dbname)) {
  int rtn = sqlite3_open(m_dbname.c_str(), &m_db);
  // test for error
  if (rtn != SQLITE_OK) {
    // TODO: Handle error
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(m_db));
    sqlite3_close(m_db);
    return;
  }

  // make sure the schema is in place
  int schemaerr = exec_sql_from_file(m_db, "sql/schema.sql");
  // TODO: what to do when the schema is broken?
}

DatabaseManager::~DatabaseManager() { sqlite3_close(m_db); }

int DatabaseManager::get_user(const std::string &username, User &user) {
  char *err_msg = 0;
  sqlite3_stmt *stmt;

  const char *sql = "SELECT id FROM Users WHERE username = ?;";
  int rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(m_db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  rc = sqlite3_step(stmt);

  if (rc == SQLITE_ROW) {
    user.id = sqlite3_column_int(stmt, 0);
    user.name = username;
  } else if (rc == SQLITE_DONE) {
    // No user found
    user.id = -1;
    user.name = "";
  } else {
    fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(m_db));
    sqlite3_finalize(stmt);
    return 1;
  }
  sqlite3_finalize(stmt);

  return 0;
}
int DatabaseManager::add_user(const std::string &username) {
  int rc{0};
  rc = get_user(username, user);
  if (user.id != -1) {
    // TODO: add call to allow for overwrite of existing user
    fprintf(stderr, "User already exists: %s\n", username.c_str());
    return 1;
  }
  if (rc != 0) {
    fprintf(stderr, "Error checking for existing user: %s\n", username.c_str());
    return 1;
  }
  char *err_msg = 0;
  sqlite3_stmt *stmt;

  const char *sql = "INSERT INTO Users VALUES (?);";
  rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(m_db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(m_db));
    return 1;
  }
  sqlite3_finalize(stmt);

  rc = get_user(username, user);
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

int DatabaseManager::add_movie(const std::string &title, const MovieType type,
                               const int release_year) {
  int rc{0};
  sqlite3_stmt *stmt;

  if (release_year == -1) {
    const char *sql =
        "INSERT OR IGNORE INTO movies (title, type) VALUES (?,?);";

    rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
  } else {
    const char *sql = "INSERT OR IGNORE INTO movies VALUES (?,?,?);";
    rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
  }
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(m_db));
    return 1;
  }

  sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
  sqlite3_bind_int(stmt, 2, type);
  if (release_year != -1) {
    sqlite3_bind_int(stmt, 3, release_year);
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_OK && rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to step statement: %s\n", sqlite3_errmsg(m_db));
    sqlite3_finalize(stmt);
    return 1;
  }
  // if (sqlite3_changes(m_db) == 0) {
  //   fprintf(stderr, "Movie already exists: %s\n", title.c_str());
  // }
  sqlite3_finalize(stmt);

  return 0;
}

int DatabaseManager::add_watchlist_entry(const int user_id, const int movie_id,
                                         std::string date, const int rating) {
  sqlite3_stmt *stmt;
  int rc{0};

  if (rating == -1) {
    const char *sql = "INSERT OR IGNORE INTO watchlist (user_id, movie_id, "
                      "date) VALUES (?,?,?);";
    rc = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
  } else {
    const char *sql = "INSERT OR IGNORE INTO watchlist VALUES (?,?,?,?);";
  }
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(m_db));
    return 1;
  }

  sqlite3_bind_int(stmt, 1, user_id);
  sqlite3_bind_int(stmt, 2, movie_id);
  sqlite3_bind_text(stmt, 3, date.c_str(), -1, SQLITE_STATIC);
  if (rating != -1) {
    sqlite3_bind_int(stmt, 4, rating);
  }
  rc = sqlite3_step(stmt);
  if (rc != SQLITE_OK && rc != SQLITE_DONE) {
    fprintf(stderr, "Failed to step statement: %s\n", sqlite3_errmsg(m_db));
    sqlite3_finalize(stmt);
    return 1;
  }

  return 0;
}
// int DatabaseManager::add_watchlist_entry(const int user_id,
//                                          const std::string &title,
//                                          const std::string date,
//                                          const int rating) {
//
//   return 0;
// }

int exec_sql_from_file(sqlite3 *db, const std::string &filename) {
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
  char *err_msg = nullptr;
  int rtn = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);

  if (rtn != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
    return -1;
  }

  return 0;
}

} // namespace core
