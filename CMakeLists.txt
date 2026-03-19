cmake_minimum_required(VERSION 3.15)
project(MovChoi CXX)

# Set C++ standard to C++20 (or 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(CURL REQUIRED)

# 1. Gather your source files
set(SOURCES
    src/main.cpp
    src/api/TMDBClient.cpp
    src/core/DatabaseManager.cpp
    src/ui/cli.cpp
    # add other .cpp files here
)

# 2. Create the executable target
add_executable(main ${SOURCES})

# add_executable(main src/main.cpp)
target_link_libraries(main
  sqlite3
  pthread
  dl
CURL::libcurl
  )

# 3. THE ELEGANT LINKING MAGIC
# This tells the compiler: "When looking for headers, start looking in the 'include' folder."
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/*)


# (Later, you will link your SQLite and HTTP libraries here)
# target_link_libraries(movie_suggester PRIVATE SQLiteCpp cpr::cpr)
# Assuming your executable target is named 'main'
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/sql"
    "${CMAKE_CURRENT_BINARY_DIR}/sql"
)
