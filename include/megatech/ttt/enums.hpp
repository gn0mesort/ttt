/**
 * @file enums.hpp
 * @brief Enumeration types for Tic-Tac-Toe.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_ENUMS_HPP
#define MEGATECH_TTT_ENUMS_HPP

#include <cinttypes>

#include <string>

namespace megatech::ttt {

  /**
   * @brief Game modes.
   */
  enum class game_mode : std::uint32_t {
    /**
     * @brief A single player game.
     */
    single_player = 0,

    /**
     * @brief A multiplayer game.
     */
    multiplayer = 0x80'00'00'00
  };

  /**
   * @brief Convert a game_mode to a string.
   * @param gm The game_mode value to be converted.
   * @return A string representing the input game_mode.
   * @throw std::runtime_error If the input game_mode was ill-formed.
   */
  std::string to_string(const game_mode gm);

  /**
   * @brief Convert a string to a game_mode value.
   * @param name The input string to be converted.
   * @return The corresponding game mode value to the input string.
   * @throw std::runtime_error If the input string does not correspond to a game_mode value.
   */
  game_mode to_game_mode(const std::string& name);

  /**
   * @brief Game phases.
   */
  enum class game_phase : std::uint32_t {
    /**
     * @brief When it is X's turn.
     */
    turn_x = 0,

    /**
     * @brief When it is O's turn.
     */
    turn_o = 0x10'00'00'00,

    /**
     * @brief When X wins the game.
     */
    win_x = 0x20'00'00'00,

    /**
     * @brief When O wins the game.
     */
    win_o = 0x30'00'00'00,

    /**
     * @brief When neither player wins the game but no more cells are available.
     */
    draw = 0x40'00'00'00
  };

  /**
   * @brief Game board cell contents.
   */
  enum class cell_contents : std::uint32_t {
    /**
     * @brief An empty cell.
     */
    empty = 0,

    /**
     * @brief A cell marked with an X.
     */
    x = 1,

    /**
     * @brief A cell marked with an O.
     */
    o = 2
  };

  /**
   * @brief Convert a game board cell's contents into a string.
   * @return "X" for cell_contents::x, "O" for cell_contents::o, and " " for cell_contents::empty.
   * @throw std::runtime_error If the input value is ill-formed.
   */
  std::string to_string(const cell_contents cc);

}

#endif
