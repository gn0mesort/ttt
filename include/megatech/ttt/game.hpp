#ifndef MEGATECH_TTT_GAME_HPP
#define MEGATECH_TTT_GAME_HPP

#include <cstddef>
#include <cinttypes>

#include <iosfwd>
#include <string>
#include <filesystem>

#include "details/lockfile.hpp"

namespace megatech::ttt {

  enum class game_mode : std::uint32_t {
    single_player = 0,
    multiplayer = 0x80'00'00'00
  };

  std::string to_string(const game_mode mode);
  game_mode to_game_mode(const std::string& name);

  enum class play_state : std::uint32_t {
    turn_x = 0,
    turn_o = 0x10'00'00'00,
    win_x = 0x20'00'00'00,
    win_o = 0x30'00'00'00,
    draw = 0x40'00'00'00
  };

  const std::filesystem::path DEFAULT_GAME_NAME{ ".ttt" };

  class game final {
  private:
    static constexpr const std::uint32_t MULTIPLAYER_BIT{ 0x80'00'00'00 };
    static constexpr const std::uint32_t PLAY_STATE_MASK{ 0x70'00'00'00 };
    static constexpr const std::uint32_t BOARD_MASK{ 0x00'03'ff'ff };
    static constexpr const std::uint32_t ALL_CELL_BITS{ 0xff };

    static constexpr const std::uint32_t  BOARD_UPPER_ROW_MASK{ 0x00'00'00'3f };
    static constexpr const std::uint32_t BOARD_MIDDLE_ROW_MASK{ 0x00'00'0f'c0 };
    static constexpr const std::uint32_t BOARD_BOTTOM_ROW_MASK{ 0x00'03'f0'00 };
    static constexpr const std::uint32_t   BOARD_LEFT_COLUMN_MASK{ 0x00'00'30'c3 };
    static constexpr const std::uint32_t BOARD_CENTER_COLUMN_MASK{ 0x00'00'c3'0c };
    static constexpr const std::uint32_t  BOARD_RIGHT_COLUMN_MASK{ 0x00'03'0c'30 };
    static constexpr const std::uint32_t BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK{ 0x00'03'03'03 };
    static constexpr const std::uint32_t BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK{ 0x00'00'33'30 };
    static constexpr const std::size_t BOARD_MIDDLE_ROW_SHIFT{ 6 };
    static constexpr const std::size_t BOARD_BOTTOM_ROW_SHIFT{ 12 };
    static constexpr const std::uint32_t   BOARD_LEFT_CELL_MASK{ 0x03 };
    static constexpr const std::uint32_t BOARD_CENTER_CELL_MASK{ 0x0c };
    static constexpr const std::uint32_t  BOARD_RIGHT_CELL_MASK{ 0x30 };
    static constexpr const std::size_t BOARD_CENTER_CELL_SHIFT{ 2 };
    static constexpr const std::size_t  BOARD_RIGHT_CELL_SHIFT{ 4 };
    static constexpr const std::uint32_t BOARD_CELL_EMPTY{ 0x00 };
    static constexpr const std::uint32_t     BOARD_CELL_X{ 0x01 };
    static constexpr const std::uint32_t     BOARD_CELL_O{ 0x02 };

    std::uint32_t m_state{ };
    std::filesystem::path m_path{ };
    details::lockfile m_lock{ ".ttt" };

    void read_data_file();
    bool is_board_full() const;
    std::uint32_t find_winner() const;
    void update_play_state();
    void take_turn(const std::size_t column, const std::size_t row, const std::uint32_t value);
  public:
    game(const std::filesystem::path& path);
    game(const std::filesystem::path& path, const game_mode mode);
    game(const game& other) = delete;
    game(game&& other) = default;

    ~game() noexcept;

    game& operator=(const game& rhs) = delete;
    game& operator=(game&& rhs) = delete;

    game_mode get_mode() const;
    play_state get_play_state() const;
    char get_cell(const std::size_t column, const std::size_t row) const;
    void take_turn(const std::size_t column, const std::size_t row);
  };

  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const game& g) {
    os << "    0  1  2 " << std::endl
       << "  0  " << g.get_cell(0, 0) << "|" << g.get_cell(1, 0) << "|" << g.get_cell(2, 0) << "  " << std::endl
       << "    ------- " << std::endl
       << "  1  " << g.get_cell(0, 1) << "|" << g.get_cell(1, 1) << "|" << g.get_cell(2, 1) << "  " << std::endl
       << "    ------- " << std::endl
       << "  2  " << g.get_cell(0, 2) << "|" << g.get_cell(1, 2) << "|" << g.get_cell(2, 2) << "  "<< std::endl;
    switch (g.get_mode())
    {
    case game_mode::single_player:
      os << "This is a single player game." << std::endl;
      break;
    case game_mode::multiplayer:
      os << "This is a multiplayer game." << std::endl;
      break;
    default:
      break;
    }
    switch (g.get_play_state())
    {
    case play_state::turn_x:
      os << "It is X's turn.";
      break;
    case play_state::turn_o:
      os << "It is O's turn.";
      break;
    case play_state::win_x:
      os << "X has won the game.";
      break;
    case play_state::win_o:
      os << "O has won the game.";
      break;
    case play_state::draw:
      os << "The game ended in a Cat's Game.";
      break;
    default:
      break;
    }
    return os;
  }

}

#endif
