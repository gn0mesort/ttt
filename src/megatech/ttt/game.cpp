#include "megatech/ttt/game.hpp"

#include <cstring>
#include <cassert>

#include <bit>
#include <stdexcept>
#include <fstream>

#include "megatech/ttt/details/data_file.hpp"

#define BSWAP32(x) \
  ((((x) & 0x00'00'00'ff) << 24) | \
    (((x) & 0x00'00'ff'00) << 8) | \
    (((x) & 0x00'ff'00'00) >> 8) | \
    (((x) & 0xff'00'00'00) >> 24))

namespace megatech::ttt {

  std::string to_string(const game_mode mode) {
    switch (mode)
    {
    case game_mode::single_player:
      return "single";
    case game_mode::multiplayer:
      return "multiplayer";
    default:
      throw std::runtime_error{ "The input game mode was not a valid enumeration value." };
    }
  }

  game_mode to_game_mode(const std::string& name) {
    if (name == to_string(game_mode::single_player))
    {
      return game_mode::single_player;
    }
    else if (name == to_string(game_mode::multiplayer))
    {
      return game_mode::multiplayer;
    }
    throw std::runtime_error{ "The input game mode was not recognized." };
  }

  void game::read_data_file() {
    auto f_in = std::ifstream{ m_path, std::ios::binary | std::ios::ate };
    if (f_in.tellg() < 0 || static_cast<std::size_t>(f_in.tellg()) < sizeof(details::data_file_header))
    {
      throw std::runtime_error{ "The requested game data file is too short to be valid." };
    }
    f_in.seekg(0, std::ios::beg);
    auto header = details::data_file_header{ };
    f_in.read(reinterpret_cast<char*>(&header), sizeof(details::data_file_header));
    if (std::memcmp(header.magic, details::DATA_FILE_HEADER_MAGIC, details::DATA_FILE_HEADER_MAGIC_LENGTH) != 0 ||
        header.version != details::DATA_FILE_VERSION_1)
    {
      throw std::runtime_error{ "The game data file is corrupt." };
    }
    auto body = details::data_file_body_v1{ };
    f_in.read(reinterpret_cast<char*>(&body), sizeof(details::data_file_body_v1));
    switch (body.endianness)
    {
    case details::DATA_FILE_REVERSE_ENDIANNESS:
      m_state = BSWAP32(body.state);
      break;
    case details::DATA_FILE_CORRECT_ENDIANNESS:
      m_state = body.state;
      break;
    default:
      throw std::runtime_error{ "The game data file is corrupt or it was written with an unknown byte order." };
    }
  }

  bool game::is_board_full() const {
    // There are 9 cells total. Each cell can be in one of three states: 00 (empty), 01 (X), 10 (O).
    // There is no 11 state.
    //
    // Therefore, if the popcount of the board is 9 or more it is full.
    return std::popcount(m_state & BOARD_MASK) >= 9;
  }

  std::uint32_t game::find_winner() const {
    const auto board = m_state & BOARD_MASK;
    // Check Rows
    switch (board & BOARD_UPPER_ROW_MASK)
    {
    case 0x00'00'00'15:
      return BOARD_CELL_X;
    case 0x00'00'00'2a:
      return BOARD_CELL_O;
    default:
      break;
    }
    switch (board & BOARD_MIDDLE_ROW_MASK)
    {
    case 0x00'00'05'40:
      return BOARD_CELL_X;
    case 0x00'00'0a'80:
      return BOARD_CELL_O;
    default:
      break;
    }
    switch (board & BOARD_BOTTOM_ROW_MASK)
    {
    case 0x00'01'50'00:
      return BOARD_CELL_X;
    case 0x00'02'a0'00:
      return BOARD_CELL_O;
    default:
      break;
    }
    // Check Columns
    switch (board & BOARD_LEFT_COLUMN_MASK)
    {
    case 0x00'00'10'41:
      return BOARD_CELL_X;
    case 0x00'00'20'82:
      return BOARD_CELL_O;
    default:
      break;
    }
    switch (board & BOARD_CENTER_COLUMN_MASK)
    {
    case 0x00'00'41'04:
      return BOARD_CELL_X;
    case 0x00'00'82'08:
      return BOARD_CELL_O;
    default:
      break;
    }
    switch (board & BOARD_RIGHT_COLUMN_MASK)
    {
    case 0x00'01'04'10:
      return BOARD_CELL_X;
    case 0x00'02'08'20:
      return BOARD_CELL_O;
    default:
      break;
    }
    // Check Diagonals
    switch (board & BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK)
    {
    case 0x00'01'01'01:
      return BOARD_CELL_X;
    case 0x00'02'02'02:
      return BOARD_CELL_O;
    default:
      break;
    }
    switch (board & BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK)
    {
    case 0x00'00'11'10:
      return BOARD_CELL_X;
    case 0x00'00'22'20:
      return BOARD_CELL_O;
    default:
      break;
    }
    return BOARD_CELL_EMPTY;
  }

  void game::update_play_state() {
    switch (find_winner())
    {
    case BOARD_CELL_X:
      m_state = (m_state & ~PLAY_STATE_MASK) | static_cast<std::uint32_t>(play_state::win_x);
      return;
    case BOARD_CELL_O:
      m_state = (m_state & ~PLAY_STATE_MASK) | static_cast<std::uint32_t>(play_state::win_o);
      return;
    default:
      break;
    }
    if (game::is_board_full())
    {
      m_state = (m_state & ~PLAY_STATE_MASK) | static_cast<std::uint32_t>(play_state::draw);
      return;
    }
    switch (get_play_state())
    {
    case play_state::turn_x:
      m_state = (m_state & ~PLAY_STATE_MASK) | static_cast<std::uint32_t>(play_state::turn_o);
      return;
    case play_state::turn_o:
      m_state = (m_state & ~PLAY_STATE_MASK) | static_cast<std::uint32_t>(play_state::turn_x);
      return;
    default:
      assert(((void) "Unreachable", false));
    }
  }

  void game::take_turn(const std::size_t column, const std::size_t row, const std::uint32_t value) {
    if (get_cell(column, row) != ' ')
    {
      throw std::runtime_error{ "The desired turn was invalid because the cell was already filled." };
    }
    auto cell = std::uint32_t{ 0 };
    switch (column)
    {
    case 0:
      cell = value;
      break;
    case 1:
      cell = value << BOARD_CENTER_CELL_SHIFT;
      break;
    case 2:
      cell = value << BOARD_RIGHT_CELL_SHIFT;
      break;
    default:
      throw std::runtime_error{ "The input column was invalid." };
    }
    auto row_bits = std::uint32_t{ 0 };
    switch (row)
    {
    case 0:
      row_bits = cell;
      break;
    case 1:
      row_bits = cell << BOARD_MIDDLE_ROW_SHIFT;
      break;
    case 2:
      row_bits = cell << BOARD_BOTTOM_ROW_SHIFT;
      break;
    default:
      throw std::runtime_error{ "The input row was invalid." };
    }
    m_state |= row_bits;
    update_play_state();
  }

  game::game(const std::filesystem::path& path) : m_path{ std::filesystem::absolute(path) }, m_lock{ m_path } {
    try
    {
      m_lock.lock();
      auto stat = std::filesystem::status(m_path);
      if (!std::filesystem::status_known(stat))
      {
        throw std::runtime_error{ "The status of the game data file could not be determined." };
      }
      if (!std::filesystem::exists(stat))
      {
        throw std::runtime_error{ "The requested game data file does not exist." };
      }
      if (!std::filesystem::is_regular_file(stat))
      {
        throw std::runtime_error{ "The requested game data file is not a regular file." };
      }
      read_data_file();
    }
    catch (...)
    {
      m_lock.unlock();
      throw;
    }
  }

  game::game(const std::filesystem::path& path, const game_mode mode) : m_path{ std::filesystem::absolute(path) },
                                                                        m_lock{ m_path } {
    try
    {
      m_lock.lock();
      auto stat = std::filesystem::status(m_path);
      if (!std::filesystem::status_known(stat))
      {
        throw std::runtime_error{ "The status of the game data file could not be determined." };
      }
      if (std::filesystem::exists(stat))
      {
        if (!std::filesystem::is_regular_file(stat))
        {
          throw std::runtime_error{ "The requested game data file is not a regular file." };
        }
        read_data_file();
      }
      m_state = 0 | static_cast<std::uint32_t>(mode);
    }
    catch (...)
    {
      m_lock.unlock();
      throw;
    }
  }

  game::~game() noexcept {
    {
      auto f_out = std::ofstream{ m_path, std::ios::binary | std::ios::trunc };
      auto header = details::data_file_header{ };
      std::memcpy(header.magic, details::DATA_FILE_HEADER_MAGIC, details::DATA_FILE_HEADER_MAGIC_LENGTH);
      header.version = details::DATA_FILE_VERSION_1;
      f_out.write(reinterpret_cast<char*>(&header), sizeof(details::data_file_header));
      auto body = details::data_file_body_v1{ };
      body.endianness = details::DATA_FILE_CORRECT_ENDIANNESS;
      body.state = m_state;
      f_out.write(reinterpret_cast<char*>(&body), sizeof(details::data_file_body_v1));
    }
    m_lock.unlock();
  }

  game_mode game::get_mode() const {
    return static_cast<game_mode>(m_state & MULTIPLAYER_BIT);
  }

  play_state game::get_play_state() const {
    return static_cast<play_state>(m_state & PLAY_STATE_MASK);
  }

  char game::get_cell(const std::size_t column, const std::size_t row) const {
    auto row_bits = std::uint32_t{ 0 };
    switch (row)
    {
    case 0:
      row_bits = m_state & BOARD_UPPER_ROW_MASK;
      break;
    case 1:
      row_bits = (m_state & BOARD_MIDDLE_ROW_MASK) >> BOARD_MIDDLE_ROW_SHIFT;
      break;
    case 2:
      row_bits = (m_state & BOARD_BOTTOM_ROW_MASK) >> BOARD_BOTTOM_ROW_SHIFT;
      break;
    default:
      throw std::runtime_error{ "The input row was invalid." };
    }
    auto cell = std::uint32_t{ 0 };
    switch (column)
    {
    case 0:
      cell = row_bits & BOARD_LEFT_CELL_MASK;
      break;
    case 1:
      cell = (row_bits & BOARD_CENTER_CELL_MASK) >> BOARD_CENTER_CELL_SHIFT;
      break;
    case 2:
      cell = (row_bits & BOARD_RIGHT_CELL_MASK) >> BOARD_RIGHT_CELL_SHIFT;
      break;
    default:
      throw std::runtime_error{ "The input column was invalid." };
    }
    switch (cell)
    {
    case BOARD_CELL_EMPTY:
      return ' ';
    case BOARD_CELL_X:
      return 'X';
    case BOARD_CELL_O:
      return 'O';
    }
    throw std::runtime_error{ "The game state was corrupted." };
  }

  void game::take_turn(const std::size_t column, const std::size_t row) {
    switch (get_play_state())
    {
    case play_state::turn_x:
      take_turn(column, row, BOARD_CELL_X);
      break;
    case play_state::turn_o:
      take_turn(column, row, BOARD_CELL_O);
      break;
    default:
      break;
    }
  }

}
