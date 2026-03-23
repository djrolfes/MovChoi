#include "core/utils.hpp"
#include "core/User.hpp"

namespace core {
std::string today() {
  const std::chrono::time_point now{std::chrono::system_clock::now()};
  const std::chrono::year_month_day ymd{
      std::chrono::floor<std::chrono::days>(now)};
  return std::to_string(static_cast<int>(ymd.year())) + "-" +
         std::to_string(static_cast<unsigned>(ymd.month())) + "-" +
         std::to_string(static_cast<unsigned>(ymd.day()));
}

void User::set_active() {} // TODO: add the active user to config file

} // namespace core
