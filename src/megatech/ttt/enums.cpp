/**
 * @file enums.cpp
 * @brief Enumeration types for Tic-Tac-Toe.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include "megatech/ttt/enums.hpp"

#include <stdexcept>

namespace megatech::ttt {

  std::string to_string(const game_mode gm) {
    switch (gm)
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

  std::string to_string(const cell_contents cc) {
    switch (cc)
    {
    case cell_contents::empty:
      return " ";
    case cell_contents::x:
      return "X";
    case cell_contents::o:
      return "O";
    default:
      throw std::runtime_error{ "The input cell content was not a valid enumeration value." };
    }
  }

}
