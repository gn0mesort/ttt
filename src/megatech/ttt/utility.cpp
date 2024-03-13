#include "megatech/ttt/utility.hpp"

#include <cstdlib>
#include <cctype>

#include <array>
#include <vector>
#include <string_view>
#include <stdexcept>
#include <locale>
#include <iostream>

#include "megatech/ttt/details/interpreter.hpp"

#include "configuration.hpp"

namespace {

#include "secret_001.inl"
#include "response_001.inl"
#include "secret_002.inl"
#include "response_002.inl"

  template <megatech::ttt::details::byte_range Range>
  std::vector<char> deobfuscate(Range&& r) {
    auto cpy = std::vector<std::ranges::range_value_t<Range>>{ std::ranges::begin(r), std::ranges::end(r) };
    megatech::ttt::details::frobnicate(cpy);
    auto interp = megatech::ttt::details::interpreter{ };
    return interp.execute(cpy);
  }

  std::string secret(const std::string& password) {
    // Secrets must not contain 0 bytes in their body.
    auto secrets = std::array<std::string, 2>{ std::string{ reinterpret_cast<const char*>(secret_001) },
                                               std::string{ reinterpret_cast<const char*>(secret_002) } };
    auto responses = std::array<std::string, 2>{ std::string{ reinterpret_cast<const char*>(response_001) },
                                                 std::string{ reinterpret_cast<const char*>(response_002) } };
    {
      auto index = 0;
      for (const auto& secret : secrets)
      {
        auto clear = deobfuscate(secret);
        if (password == std::string_view{ clear.data(), clear.size() })
        {
          clear = deobfuscate(responses[index]);
          return std::string{ std::string_view{ clear.data(), clear.size() } };
        }
        ++index;
      }
    }
    return "";
  }

}

namespace megatech::ttt {

  std::uint32_t initialize(const int argc, const char *const *const argv) {
    std::locale::global(std::locale{ "" });
    if (argc < 1)
    {
      return 1;
    }
    for (auto i = 1; i < argc; ++i)
    {
      if (auto res = secret(argv[i]); !res.empty())
      {
        std::cerr << res << std::endl;
        return 0xff'ff'ff'ff;
      }
    }
    return 0;
  }


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
    if (home_path.empty())
    {
      throw std::runtime_error{ "The user's home directory could not be detected." };
    }
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

  std::string tolower(const std::string& str) {
    auto cpy = str;
    for (auto& c : cpy)
    {
      c = static_cast<char>(static_cast<unsigned char>(std::tolower(c)));
    }
    return cpy;
  }


}
