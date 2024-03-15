/**
 * @file utility.hpp
 * @brief Utility functions for Tic-Tac-Toe.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_UTILITY_HPP
#define MEGATECH_TTT_UTILITY_HPP

#include <cstddef>
#include <cinttypes>

#include <filesystem>
#include <concepts>
#include <ranges>
#include <string>

namespace megatech::ttt::details {

  /**
   * @brief A concept that is satisfied when Type is any of the types in Desired.
   * @tparam Type The input type.
   * @tparam Desired A set of desired types to match Type against.
   */
  template <typename Type, typename... Desired>
  concept is_any_of = (std::same_as<Type, Desired> || ...);

  /**
   * @brief A concept that is satisfied when Type is a forward_range of characters or bytes.
   * @tparam Type The input type.
   */
  template <typename Type>
  concept byte_range = std::ranges::forward_range<Type> &&
          is_any_of<std::ranges::range_value_t<Type>, char, unsigned char, std::byte>;

  /**
   * @brief A standard compatible replacement for GNU memfrob.
   * @details Like the equivalent GNU function, this "encrypts" a region of memory by XORing each byte with the value
   *          42. The input range must be a forward iterable sequence of bytes. That means the value type of the
   *          range MUST be char, unsigned char, or std::byte. The bytes are modified in place.
   * @tparam Range The type of the input range. It must always be a byte_range.
   * @param r The input range.
   */
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

  /**
   * @brief A convenient application initialization function for Tic-Tac-Toe applications.
   * @details This sets the locale to the user preferred locale and handles special input arguments.
   * @param argc The number of program arguments as if taken from main.
   * @param argv The list of program argument strings as if taken from main.
   * @return An unsigned integer value. A return value of 0 indicates successful initialization. Positive values
   *         indicate that the program should terminate.
   */
  std::uint32_t initialize(const int argc, const char *const *const argv);

  /**
   * @brief A function to find the user home directory.
   * @details This attempts to find a home directory belonging to the current user. There are basically two paths
   *          for this function. First, the function will attempt to read the environment variable
   *          MEGATECH_TTT_HOME. If that value is set then it will be treated as the authoritative home location.
   *          Second, the function will check a system specific location. For POSIX builds this is the HOME environment
   *          variable. For Windows, USERPROFILE is checked followed by HOMEPATH. Errors are thrown if a valid path
   *          can't be found in the environment.
   * @return A filesystem path indicating the detected home directory.
   * @throw std::runtime_error If no path is found or if the detected path is invalid in some way.
   */
  std::filesystem::path find_home_directory();

  /**
   * @brief Converts a string to all lowercase characters in the current locale.
   * @param str The string to convert.
   * @return A new string equivalent to the input string after case conversion.
   */
  std::string tolower(const std::string& str);

}

#endif
