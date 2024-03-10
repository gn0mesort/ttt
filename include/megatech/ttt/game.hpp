#ifndef MEGATECH_TTT_GAME_HPP
#define MEGATECH_TTT_GAME_HPP

#include <cstddef>
#include <cinttypes>

#include <iosfwd>
#include <string>
#include <filesystem>

#include "enums.hpp"

#include "details/lockfile.hpp"
#include "details/state.hpp"

namespace megatech::ttt {

  const std::filesystem::path DEFAULT_GAME_NAME{ ".ttt" };

  class game final {
  private:
    details::state m_state{ };
    std::filesystem::path m_path{ };
    details::lockfile m_lock{ ".ttt" };

    void read_data_file();
    cell_contents find_winner() const;
    void update_play_state();
    void take_turn(const std::size_t column, const std::size_t row, const cell_contents value);
  public:
    game(const std::filesystem::path& path);
    game(const std::filesystem::path& path, const game_mode mode);
    game(const game& other) = delete;
    game(game&& other) = default;

    ~game() noexcept;

    game& operator=(const game& rhs) = delete;
    game& operator=(game&& rhs) = delete;

    const details::state& state() const;

    void take_turn(const std::size_t column, const std::size_t row);
  };

  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const game& g) {
    const auto& st = g.state();
    os << "    0  1  2 " << std::endl
       << "  0  " << to_string(st.cell(0, 0)) << "|" << to_string(st.cell(1, 0)) << "|" << to_string(st.cell(2, 0))
       << "  " << std::endl << "    ------- " << std::endl
       << "  1  " << to_string(st.cell(0, 1)) << "|" << to_string(st.cell(1, 1)) << "|" << to_string(st.cell(2, 1))
       << "  " << std::endl << "    ------- " << std::endl
       << "  2  " << to_string(st.cell(0, 2)) << "|" << to_string(st.cell(1, 2)) << "|" << to_string(st.cell(2, 2))
       << "  "<< std::endl;
    switch (st.mode())
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
    switch (st.phase())
    {
    case game_phase::turn_x:
      os << "It is X's turn.";
      break;
    case game_phase::turn_o:
      os << "It is O's turn.";
      break;
    case game_phase::win_x:
      os << "X has won the game.";
      break;
    case game_phase::win_o:
      os << "O has won the game.";
      break;
    case game_phase::draw:
      os << "The game ended in a Cat's Game.";
      break;
    default:
      break;
    }
    return os;
  }

}

#endif
