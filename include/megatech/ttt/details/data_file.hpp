/**
 * @file data_file.hpp
 * @brief Helper definitions for handling game data files.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_DETAILS_DATA_FILE_HPP
#define MEGATECH_TTT_DETAILS_DATA_FILE_HPP

#include <cstddef>
#include <cinttypes>

namespace megatech::ttt::details {

  constexpr const std::size_t DATA_FILE_HEADER_MAGIC_LENGTH{ 8 };

  /**
   * @brief The magic string used to verify the identity of game data files.
   * @details This is a string that *SHOULD* be equivalent to "\x89ttt\r\n\x1a\n" (minus the null terminator).
  *           Like the PNG header string, which is almost identical, the main purpose of this is to detect file
  *           reading errors. The first character (0x89) protects against systems the only handle 7-bit ASCII. Next,
  *           the application identifier "ttt" appears. This is followed by a Windows-style line-ending. If this is
  *           replaced by "\n" then it's obvious the file was opened incorrectly (i.e., Windows->POSIX line-ending
  *           conversion occurred). The next character (0x1a) is a DOS-style EOF character which should prevent
  *           accidently printing the binary data in a DOS terminal via type. Finally, there's a usual POSIX-style
  *           line-ending which serves the same purpose as the Windows-style line-ending (i.e., checking if
  *           POSIX->Windows line-ending conversion occurred).
  *
  *           Probably this is a little paranoid.
  */
  constexpr unsigned char DATA_FILE_HEADER_MAGIC[DATA_FILE_HEADER_MAGIC_LENGTH]{ 0x89, 0x74, 0x74, 0x74, 0x0d, 0x0a,
                                                                                 0x1a, 0x0a };

  /**
   * @brief The version value for version 1 data file bodies.
   */
  constexpr unsigned char DATA_FILE_VERSION_1{ 1 };

  /**
   * @brief The generic header for all game data files.
   * @details This is a packed structure.
   */
  struct data_file_header final {
    /**
     * @brief The magic header string.
     */
    unsigned char magic[DATA_FILE_HEADER_MAGIC_LENGTH];

    /**
     * @brief The version of the file.
     */
    unsigned char version;
  };

  /**
   * @brief The correct endianness check value for file bodies.
   * @details Regardless of the system endianness, this should be the value when bytes are read into memory.
   */
  constexpr std::uint32_t DATA_FILE_CORRECT_ENDIANNESS{ 0xaa'bb'cc'dd };

  /**
   * @brief The reverse endianness check value for file bodies.
   * @details When a big-endian game data file is read on a little-endian system (or vice-versa) this should be the
   *          value in memory. When this value is detected bytes will be swapped to correct the data.
   */
  constexpr std::uint32_t DATA_FILE_REVERSE_ENDIANNESS{ 0xdd'cc'bb'aa };

  /**
   * @brief The body for version 1 game data files.
   * @details This is a packed structure.
   */
  struct data_file_body_v1 final {
    /**
     * @brief The endianness check value.
     */
    std::uint32_t endianness;

    /**
     * @brief The current game state.
     */
    std::uint32_t state;
  };

}

#endif
