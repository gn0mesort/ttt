/**
 * @file state.cpp
 * @brief Game state management object.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include "megatech/ttt/details/state.hpp"

#include <bit>
#include <stdexcept>
#include <array>
#include <utility>

namespace megatech::ttt::details {

  state::state(const std::uint32_t data) : m_data{ data } {
    // There are five game phases. Since the enum values are zero based, this means that any value greater than four
    // in the phase bits is invalid.
    if ((m_data & GAME_PHASE_MASK) >= 0x50'00'00'00)
    {
      throw std::runtime_error{ "The game phase was invalid." };
    }
    // There is a group of unused bits in between the game phase and the board data. If any of these bits are set then
    // the game state is corrupt.
    if (m_data & GAME_UNUSED_MASK)
    {
      throw std::runtime_error{ "The game state is corrupt." };
    }
    // Each cell in the game board can be empty, filled with an X, or filled with an O. This means there are only three
    // valid states. The fourth state (0x03) is invalid even though it occupies the same space.
    //
    // This loop runs at most 9 times. The number of loops required is determined by how many cells are filled.
    for (auto board = m_data & GAME_BOARD_MASK; board != 0; board >>= 2)
    {
      if ((board & ALL_CELL_BITS) == ALL_CELL_BITS)
      {
        throw std::runtime_error{ "The game board contained an invalid cell." };
      }
    }
  }

  state::operator std::uint32_t() const {
    return m_data;
  }

  game_mode state::mode() const {
    return static_cast<game_mode>(m_data & GAME_MODE_MASK);
  }

  void state::mode(const game_mode gm) {
    const auto gm_bits = static_cast<std::uint32_t>(gm);
    if (gm_bits != 0x80'00'00'00 && gm_bits != 0)
    {
      throw std::runtime_error{ "The game mode was invalid." };
    }
    m_data = (m_data & ~GAME_MODE_MASK) | static_cast<std::uint32_t>(gm);
  }

  game_phase state::phase() const {
    return static_cast<game_phase>(m_data & GAME_PHASE_MASK);
  }

  void state::phase(const game_phase gp) {
    const auto gp_bits = static_cast<std::uint32_t>(gp);
    if (gp_bits >= 0x50'00'00'00)
    {
      throw std::runtime_error{ "The game phase was invalid." };
    }
    m_data = (m_data & ~GAME_PHASE_MASK) | gp_bits;
  }

  std::uint32_t state::board() const {
    return m_data & GAME_BOARD_MASK;
  }

  bool state::is_board_empty() const {
    return filled_cells() == 0;
  }

  bool state::is_board_full() const {
    return filled_cells() >= 9;
  }

  bool state::is_row_x(const std::size_t row) const {
    switch (row)
    {
    case 0:
      return (m_data & BOARD_UPPER_ROW_MASK) == 0x00'00'00'15;
    case 1:
      return (m_data & BOARD_MIDDLE_ROW_MASK) == 0x00'00'05'40;
    case 2:
      return (m_data & BOARD_BOTTOM_ROW_MASK) == 0x00'01'50'00;
    default:
      throw std::runtime_error{ "The row index is out of bounds." };
    }
  }

  bool state::is_row_o(const std::size_t row) const {
    switch (row)
    {
    case 0:
      return (m_data & BOARD_UPPER_ROW_MASK) == 0x00'00'00'2a;
    case 1:
      return (m_data & BOARD_MIDDLE_ROW_MASK) == 0x00'00'0a'80;
    case 2:
      return (m_data & BOARD_BOTTOM_ROW_MASK) == 0x00'02'a0'00;
    default:
      throw std::runtime_error{ "The row index is out of bounds." };
    }
  }

  bool state::is_column_x(const std::size_t column) const {
    switch (column)
    {
    case 0:
      return (m_data & BOARD_LEFT_COLUMN_MASK) == 0x00'00'10'41;
    case 1:
      return (m_data & BOARD_CENTER_COLUMN_MASK) == 0x00'00'41'04;
    case 2:
      return (m_data & BOARD_RIGHT_COLUMN_MASK) == 0x00'01'04'10;
    default:
      throw std::runtime_error{ "The column index is out of bounds." };
    }
  }

  bool state::is_column_o(const std::size_t column) const {
    switch (column)
    {
    case 0:
      return (m_data & BOARD_LEFT_COLUMN_MASK) == 0x00'00'20'82;
    case 1:
      return (m_data & BOARD_CENTER_COLUMN_MASK) == 0x00'00'82'08;
    case 2:
      return (m_data & BOARD_RIGHT_COLUMN_MASK) == 0x00'02'08'20;
    default:
      throw std::runtime_error{ "The column index is out of bounds." };
    }
  }

  bool state::is_left_diagonal_x() const {
    return (m_data & BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK) == 0x00'01'01'01;
  }

  bool state::is_left_diagonal_o() const {
    return (m_data & BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK) == 0x00'02'02'02;
  }

  bool state::is_right_diagonal_x() const {
    return (m_data & BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK) == 0x00'00'11'10;
  }

  bool state::is_right_diagonal_o() const {
    return (m_data & BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK) == 0x00'00'22'20;
  }

  bool state::is_cell_empty(const std::size_t column, const std::size_t row) const {
    return cell(column, row) == cell_contents::empty;
  }

  bool state::is_cell_x(const std::size_t column, const std::size_t row) const {
    return cell(column, row) == cell_contents::x;
  }

  std::size_t state::filled_cells() const {
    return std::popcount(board());
  }

  std::size_t state::count_x() const {
    return std::popcount(m_data & COUNT_X_MASK);
  }

  std::size_t state::count_o() const {
    return std::popcount(m_data & COUNT_O_MASK);
  }

  cell_contents state::cell(const std::size_t column, const std::size_t row) const {
    if (column > 2)
    {
      throw std::runtime_error{ "The column index is out of bounds." };
    }
    if (row > 2)
    {
      throw std::runtime_error{ "The row index is out of bounds." };
    }
    const auto row_masks = std::array<std::uint32_t, 3>{ BOARD_UPPER_ROW_MASK, BOARD_MIDDLE_ROW_MASK,
                                                         BOARD_BOTTOM_ROW_MASK };
    const auto row_shifts = std::array<std::size_t, 3>{ 0, BOARD_MIDDLE_ROW_SHIFT, BOARD_BOTTOM_ROW_SHIFT };
    const auto cell_masks = std::array<std::uint32_t, 3>{ BOARD_LEFT_CELL_MASK, BOARD_CENTER_CELL_MASK,
                                                          BOARD_RIGHT_CELL_MASK };
    const auto cell_shifts = std::array<std::size_t, 3>{ 0, BOARD_CENTER_CELL_SHIFT, BOARD_RIGHT_CELL_SHIFT };
    const auto entire_row = (m_data & row_masks[row]) >> row_shifts[row];
    return static_cast<cell_contents>((entire_row & cell_masks[column]) >> cell_shifts[column]);
  }

  void state::cell(const std::size_t column, const std::size_t row, cell_contents value) {
    if (column > 2)
    {
      throw std::runtime_error{ "The column index is out of bounds." };
    }
    if (row > 2)
    {
      throw std::runtime_error{ "The row index is out of bounds." };
    }
    const auto row_shifts = std::array<std::size_t, 3>{ 0, BOARD_MIDDLE_ROW_SHIFT, BOARD_BOTTOM_ROW_SHIFT };
    const auto cell_shifts = std::array<std::size_t, 3>{ 0, BOARD_CENTER_CELL_SHIFT, BOARD_RIGHT_CELL_SHIFT };
    const auto clear_cell = ALL_CELL_BITS << (cell_shifts[column] + row_shifts[row]);
    m_data &= ~clear_cell;
    const auto value_bits = static_cast<std::uint32_t>(value) << (cell_shifts[column] + row_shifts[row]);
    m_data |= value_bits;
  }
}
