#pragma once

#include <cstdlib>
#include <stdexcept>
#include <string>

namespace api {

class TMDBClient {
public:
  static TMDBClient
  fromEnvironment(const std::string &apiKeyVar = "THEMOVIEDATABASE_API_KEY",
                  const std::string &tokenVar = "THEMOVIEDATABASE_ACCESSTOKEN");

  const std::string &getApiKey() const { return m_apiKey; }
  const std::string &getAccessToken() const { return m_accessToken; }

private:
  TMDBClient(std::string apiKey, std::string accessToken)
      : m_apiKey(std::move(apiKey)), m_accessToken(std::move(accessToken)) {}

  static std::string getEnvOrThrow(const std::string &varName);

  std::string m_apiKey;
  std::string m_accessToken;
  const std::string m_baseUrl = "https://api.themoviedb.org/3";
};

} // namespace api
