#pragma once

#include <string>
namespace core {
struct User {
  int id = -1;
  std::string name;
  // std::string email;
  // std::string passwordHash; // Store hashed passwords, not plain text
};
} // namespace core
