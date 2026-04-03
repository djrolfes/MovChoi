cmake_minimum_required(VERSION 3.15)
project(
  MovChoi CXX
  LANGUAGES CXX
  VERSION 1.0.0)

# Set C++ standard to C++20 (or 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(CURL REQUIRED)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(FETCHCONTENT_QUIET OFF)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9)
FetchContent_MakeAvailable(ftxui)
# ------------------------------------------------------------------------------
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0 # Matches your requested version
)
FetchContent_MakeAvailable(nlohmann_json)
# find_package(nlohmann_json 3.12.0 REQUIRED) add_library(foo ...)
# target_link_libraries(foo PRIVATE nlohmann_json::nlohmann_json)
set(CLI11_PRECOMPILED ON)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/CLI11)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/SQLiteCpp)

# 1. Gather your source files
set(SOURCES
    src/main.cpp src/api/TMDBClient.cpp src/core/DatabaseManager.cpp
    src/ui/tui.cpp
    # add other .cpp files here
)

# 1. Create the executable target
add_executable(main ${SOURCES})

# add_executable(main src/main.cpp)
target_link_libraries(
  main
  PRIVATE SQLiteCpp sqlite3 pthread dl
  PRIVATE CURL::libcurl
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
  PRIVATE nlohmann_json::nlohmann_json
  PRIVATE CLI11::CLI11)

# 1. THE ELEGANT LINKING MAGIC This tells the compiler: "When looking for
#   headers, start looking in the 'include' folder."
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/*)

# (Later, you will link your SQLite and HTTP libraries here)
# target_link_libraries(movie_suggester PRIVATE SQLiteCpp cpr::cpr) Assuming
# your executable target is named 'main'
add_custom_command(
  TARGET main
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/sql"
          "${CMAKE_CURRENT_BINARY_DIR}/sql")
