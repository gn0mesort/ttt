#include "megatech/ttt/utility.hpp"

#include <cstdlib>

#include <stdexcept>

#include "configuration.hpp"

namespace megatech::ttt {

  std::filesystem::path find_home_directory() {
    auto home_path = std::filesystem::path{ };
    if (auto ttt_home = std::getenv("MEGATECH_TTT_HOME"); ttt_home)
    {
      home_path = std::filesystem::absolute(ttt_home);
      goto done;
    }
#if defined(CONFIGURATION_OPERATING_SYSTEM_POSIX)
    if (auto posix_home = std::getenv("HOME"); posix_home)
    {
      home_path = std::filesystem::absolute(posix_home);
    }
#elif defined(CONFIGURATION_OPERATING_SYSTEM_WINDOWS)
    if (auto windows_home = std::getenv("USERPROFILE"); windows_home)
    {
      home_path = std::filesystem::absolute(windows_home);
      goto done;
    }
    if (auto windows_home = std::getenv("HOMEPATH"); windows_home)
    {
      home_path = std::filesystem::absolute(windows_home);
    }
#endif
done:
    auto stat = std::filesystem::status(home_path);
    if (!std::filesystem::status_known(stat))
    {
      throw std::runtime_error{ "The user's home directory could not be detected." };
    }
    if (!std::filesystem::exists(stat))
    {
      throw std::runtime_error{ "The detected home directory does not exist." };
    }
    if (!std::filesystem::is_directory(stat))
    {
      throw std::runtime_error{ "The detected home directory was not actually a directory." };
    }
    return home_path;
  }

}
