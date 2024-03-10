#ifndef MEGATECH_TTT_ENUMS_HPP
#define MEGATECH_TTT_ENUMS_HPP

#include <cinttypes>

#include <string>

namespace megatech::ttt {

  enum class game_mode : std::uint32_t {
    single_player = 0,
    multiplayer = 0x80'00'00'00
  };

  std::string to_string(const game_mode gm);
  game_mode to_game_mode(const std::string& name);

  enum class game_phase : std::uint32_t {
    turn_x = 0,
    turn_o = 0x10'00'00'00,
    win_x = 0x20'00'00'00,
    win_o = 0x30'00'00'00,
    draw = 0x40'00'00'00
  };

  enum class cell_contents : std::uint32_t {
    empty = 0,
    x = 1,
    o = 2
  };

  std::string to_string(const cell_contents cc);

}

#endif
