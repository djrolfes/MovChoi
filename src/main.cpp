#include "api/TMDBClient.hpp"
#include "ui/cli.hpp"
#include "ui/tui.hpp"
#include <cstdio>
// #include <curl/curl.h>
#include <string>

int main(int argc, char *argv[]) {
  ui::TUI tui = ui::TUI();
  tui.init();
  tui.run();
  // ui::CLI cli = ui::CLI::fromArgs(argc, argv);

  // setAPIKey();
  // setAccessToken();
  // CURL *hnd = curl_easy_init();
  //
  // curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
  // curl_easy_setopt(hnd, CURLOPT_WRITEDATA, stdout);
  // curl_easy_setopt(hnd, CURLOPT_URL,
  //                  "https://api.themoviedb.org/3/configuration");
  //
  // std::string AuthString = "Authorization: Bearer " + TMDB_ACCESS_TOKEN;
  // struct curl_slist *headers = NULL;
  // headers = curl_slist_append(headers, "accept: application/json");
  // headers = curl_slist_append(headers, AuthString.c_str());
  // printf("%s\n", TMDB_ACCESS_TOKEN.c_str());
  // curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);
  //
  // CURLcode ret = curl_easy_perform(hnd);
  return 0;
}
