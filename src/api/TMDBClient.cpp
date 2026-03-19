#include "api/TMDBClient.hpp"

#include <cstdlib>
#include <stdexcept>

namespace api {

TMDBClient TMDBClient::fromEnvironment(const std::string &apiKeyVar,
                                       const std::string &tokenVar) {
  std::string apiKey = getEnvOrThrow(apiKeyVar);
  std::string accessToken = getEnvOrThrow(tokenVar);
  return TMDBClient(std::move(apiKey), std::move(accessToken));
}

std::string TMDBClient::getEnvOrThrow(const std::string &varName) {
  const char *value = std::getenv(varName.c_str());
  if (!value) {
    throw std::runtime_error("Environment variable " + varName + " not set");
  }
  return std::string(value);
}

} // namespace api
