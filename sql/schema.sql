-- sqlfluff:dialect:sqlite
CREATE TABLE IF NOT EXISTS users (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT UNIQUE
);

CREATE TABLE IF NOT EXISTS watchlist (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    userid INTEGER NOT NULL,
    movieid INTEGER NOT NULL,
    watchdate DATE,
    rating INTEGER,
    FOREIGN KEY (userid) REFERENCES users (id) ON DELETE CASCADE,
    FOREIGN KEY (movieid) REFERENCES movies (id) ON DELETE CASCADE,
    UNIQUE (userid, movieid)
);

CREATE TABLE IF NOT EXISTS movies (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    title TEXT NOT NULL,
    type INTEGER,
    release_year INTEGER,
    UNIQUE (title, release_year)
);

CREATE TABLE IF NOT EXISTS genres (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT UNIQUE NOT NULL
);

CREATE TABLE IF NOT EXISTS movie_genres (
    movie_id INTEGER,
    genre_id INTEGER,
    PRIMARY KEY (movie_id, genre_id),
    FOREIGN KEY (movie_id) REFERENCES movies (id) ON DELETE CASCADE,
    FOREIGN KEY (genre_id) REFERENCES genres (id) ON DELETE CASCADE
);
