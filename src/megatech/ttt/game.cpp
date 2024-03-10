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
      m_state = details::state{ BSWAP32(body.state) };
      break;
    case details::DATA_FILE_CORRECT_ENDIANNESS:
      m_state = details::state{ body.state };
      break;
    default:
      throw std::runtime_error{ "The game data file is corrupt or it was written with an unknown byte order." };
    }
  }


  cell_contents game::find_winner() const {
    for (auto i = 0; i < 3; ++i)
    {
      if (m_state.is_row_x(i))
      {
        return cell_contents::x;
      }
      if (m_state.is_row_o(i))
      {
        return cell_contents::o;
      }
    }
    for (auto i = 0; i < 3; ++i)
    {
      if (m_state.is_column_x(i))
      {
        return cell_contents::x;
      }
      if (m_state.is_column_o(i))
      {
        return cell_contents::o;
      }
    }
    if (m_state.is_left_diagonal_x() || m_state.is_right_diagonal_x())
    {
      return cell_contents::x;
    }
    if (m_state.is_left_diagonal_o() || m_state.is_right_diagonal_o())
    {
      return cell_contents::o;
    }
    return cell_contents::empty;
  }

  void game::update_play_state() {
    switch (find_winner())
    {
    case cell_contents::x:
      m_state.phase(game_phase::win_x);
      return;
    case cell_contents::o:
      m_state.phase(game_phase::win_o);
      return;
    default:
      break;
    }
    if (m_state.is_board_full())
    {
      m_state.phase(game_phase::draw);
      return;
    }
    switch (m_state.phase())
    {
    case game_phase::turn_x:
      m_state.phase(game_phase::turn_o);
      return;
    case game_phase::turn_o:
      m_state.phase(game_phase::turn_x);
      return;
    default:
      assert(((void) "Unreachable", false));
    }
  }

  void game::take_turn(const std::size_t column, const std::size_t row, const cell_contents value) {
    if (m_state.cell(column, row) != cell_contents::empty)
    {
      throw std::runtime_error{ "The desired turn was invalid because the cell was already filled." };
    }
    m_state.cell(column, row, value);
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
      m_state = details::state{ 0 | static_cast<std::uint32_t>(mode) };
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
      body.state = static_cast<std::uint32_t>(m_state);
      f_out.write(reinterpret_cast<char*>(&body), sizeof(details::data_file_body_v1));
    }
    m_lock.unlock();
  }

  void game::take_turn(const std::size_t column, const std::size_t row) {
    switch (m_state.phase())
    {
    case game_phase::turn_x:
      take_turn(column, row, cell_contents::x);
      break;
    case game_phase::turn_o:
      take_turn(column, row, cell_contents::o);
      break;
    default:
      break;
    }
  }

  const details::state& game::state() const {
    return m_state;
  }

}
