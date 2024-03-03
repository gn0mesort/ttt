#ifndef MEGATECH_TTT_GAME_FILE_HPP
#define MEGATECH_TTT_GAME_FILE_HPP

#include <cstddef>
#include <cinttypes>

#include <filesystem>

namespace megatech::ttt {

  class lockfile;
  class scoped_file_lock;

  class game_file final {
  private:
    static constexpr const std::size_t HEADER_MAGIC_LENGTH{ 8 };

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
    static constexpr const unsigned char HEADER_MAGIC[HEADER_MAGIC_LENGTH]{ 0x89, 0x74, 0x74, 0x74,
                                                                            0x0d, 0x0a, 0x1a, 0x0a };

    static constexpr const std::uint32_t MULTIPLAYER_BIT{ 0x01'00'00'00 };

    static constexpr const std::uint32_t GAME_STATE_MASK{ 0x06'00'00'00 };
    static constexpr const std::uint32_t   CATS_GAME_BIT{ 0x02'00'00'00 };
    static constexpr const std::uint32_t       X_WIN_BIT{ 0x04'00'00'00 };
    static constexpr const std::uint32_t       O_WIN_BIT{ 0x06'00'00'00 };

    static constexpr const std::uint32_t   BOARD_CELL_UPPER_LEFT_MASK{ 0x00'00'00'03 };
    static constexpr const std::uint32_t BOARD_CELL_UPPER_CENTER_MASK{ 0x00'00'00'0c };
    static constexpr const std::uint32_t  BOARD_CELL_UPPER_RIGHT_MASK{ 0x00'00'00'30 };

    static constexpr const std::uint32_t   BOARD_CELL_MIDDLE_LEFT_MASK{ 0x00'00'00'c0 };
    static constexpr const std::uint32_t BOARD_CELL_MIDDLE_CENTER_MASK{ 0x00'00'03'00 };
    static constexpr const std::uint32_t  BOARD_CELL_MIDDLE_RIGHT_MASK{ 0x00'00'0c'00 };

    static constexpr const std::uint32_t   BOARD_CELL_BOTTOM_LEFT_MASK{ 0x00'00'30'00 };
    static constexpr const std::uint32_t BOARD_CELL_BOTTOM_CENTER_MASK{ 0x00'00'c0'00 };
    static constexpr const std::uint32_t  BOARD_CELL_BOTTOM_RIGHT_MASK{ 0x00'03'00'00 };

    struct header final {
      // This should *ALWAYS* compare equal to HEADER_MAGIC.
      unsigned char magic[HEADER_MAGIC_LENGTH];
      // I don't anticipate more than one file version, and I certainly don't anticipate more than 255.
      // Currently this must *ALWAYS* be 1.
      unsigned char version;
    };

    static constexpr const std::uint32_t CORRECT_ENDIANNESS{ 0xaa'bb'cc'dd };
    static constexpr const std::uint32_t REVERSE_ENDIANNESS{ 0xdd'cc'bb'aa };

    struct body_v1 final {
      // This should always be: 0xaa'bb'cc'dd. If it isn't then we may need to byteswap the state.
      std::uint32_t endianness;
      std::uint32_t state;
    };

    std::filesystem::path m_path{ };
    lockfile* m_lock{ };
    scoped_file_lock* m_lock_guard{ };
    header m_file_header{ };
    body_v1 m_file_body{ };
  public:
    game_file(const std::filesystem::path& path);
    game_file(const game_file& other) = delete;
    game_file(game_file&& other) = default;

    ~game_file() noexcept;

    game_file& operator=(const game_file& rhs) = delete;
    game_file& operator=(game_file&& rhs) = default;
  };

}

#endif
