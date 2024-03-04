#ifndef MEGATECH_TTT_DETAILS_DATA_FILE_HPP
#define MEGATECH_TTT_DETAILS_DATA_FILE_HPP

#include <cstddef>
#include <cinttypes>

namespace megatech::ttt::details {

  constexpr const std::size_t DATA_FILE_HEADER_MAGIC_LENGTH{ 8 };

  // This is a string that *SHOULD* be equivalent to "\x89ttt\r\n\x1a\n" (minus the null terminator).
  // Like the PNG header string, which is almost identical, the main purpose of this is to detect file reading
  // errors. The first character (0x89) protects against systems the only handle 7-bit ASCII. Next, the application
  // identifier "ttt" appears. This is followed by a Windows-style line-ending. If this is replaced by "\n" then
  // it's obvious the file was opened incorrectly (i.e., Windows->POSIX line-ending conversion occurred). The next
  // character (0x1a) is a DOS-style EOF character which should prevent accidently printing the binary data in a
  // DOS terminal via type. Finally, there's a usual POSIX-style line-ending which serves the same purpose as the
  // Windows-style line-ending (i.e., checking if POSIX->Windows line-ending conversion occurred).
  //
  // Probably this is a little paranoid.
  constexpr const unsigned char DATA_FILE_HEADER_MAGIC[DATA_FILE_HEADER_MAGIC_LENGTH]{ 0x89, 0x74, 0x74, 0x74,
                                                                                       0x0d, 0x0a, 0x1a, 0x0a };

  // I don't anticipate future versions but
  constexpr const unsigned char DATA_FILE_VERSION_1{ 1 };

  struct data_file_header final {
    unsigned char magic[DATA_FILE_HEADER_MAGIC_LENGTH];
    unsigned char version;
  };

  constexpr const std::uint32_t CORRECT_ENDIANNESS{ 0xaa'bb'cc'dd };
  constexpr const std::uint32_t REVERSE_ENDIANNESS{ 0xdd'cc'bb'aa };

  struct data_file_body_v1 final {
    std::uint32_t endianness;
    std::uint32_t state;
  };

}

#endif
