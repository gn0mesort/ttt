#ifndef MEGATECH_TTT_UTILITY_HPP
#define MEGATECH_TTT_UTILITY_HPP

#include <cstddef>
#include <cinttypes>

#include <filesystem>
#include <concepts>
#include <ranges>
#include <string>

namespace megatech::ttt::details {

  template <typename Type, typename... Desired>
  concept is_any_of = (std::same_as<Type, Desired> || ...);

  template <typename Type>
  concept byte_range = std::ranges::forward_range<Type> &&
          is_any_of<std::ranges::range_value_t<Type>, char, unsigned char, std::byte>;

  template <details::byte_range Range>
  void frobnicate(Range&& r) {
    for (auto cur = std::ranges::begin(r); cur != std::ranges::end(r); ++cur)
    {
      // Per memfrob(3), the key value is 42.
      // I'm sure someone thought it was a clever choice.
      *cur = *cur ^ 42;
    }
  }

}

namespace megatech::ttt {

  std::uint32_t initialize(const int argc, const char *const *const argv);

  void initialize_preferred_locale();

  std::filesystem::path find_home_directory();


  std::string tolower(const std::string& str);

}

#endif
