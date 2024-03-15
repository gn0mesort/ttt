/**
 * @file strategy.cpp
 * @brief Tic-Tac-Toe CPU player strategy objects.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include "megatech/ttt/strategy.hpp"

#include <stdexcept>

#include "megatech/ttt/game.hpp"

namespace {

  bool is_valid(const megatech::ttt::cell_location& location) {
    return location.column < 3 && location.row < 3;
  }

}

namespace megatech::ttt {

  cell_location strategy::find_edge(const details::state& st) const {
    // Edge cells are neither a corner nor the center cell.
    auto available = std::vector<cell_location>{ };
    if (st.is_cell_empty(1, 0))
    {
      available.push_back({ 1, 0 });
    }
    if (st.is_cell_empty(0, 1))
    {
      available.push_back({ 0, 1 });
    }
    if (st.is_cell_empty(1, 2))
    {
      available.push_back({ 1, 2 });
    }
    if (st.is_cell_empty(2, 1))
    {
      available.push_back({ 2, 1 });
    }
    if (available.empty())
    {
      return INVALID_LOCATION;
    }
    auto selector = std::uniform_int_distribution<std::size_t>{ 0, available.size() - 1 };
    return available[selector(m_prng)];
  }

  cell_location strategy::find_corner(const details::state& st) const {
    auto available = std::vector<cell_location>{ };
    if (st.is_cell_empty(0, 0))
    {
      available.push_back({ 0, 0 });
    }
    if (st.is_cell_empty(0, 2))
    {
      available.push_back({ 0, 2 });
    }
    if (st.is_cell_empty(2, 2))
    {
      available.push_back({ 2, 2 });
    }
    if (st.is_cell_empty(2, 0))
    {
      available.push_back({ 2, 0 });
    }
    if (available.empty())
    {
      return INVALID_LOCATION;
    }
    auto selector = std::uniform_int_distribution<std::size_t>{ 0, available.size() - 1 };
    return available[selector(m_prng)];
  }

  cell_location strategy::find_opposite_corner(const details::state& st, const cell_location& last) const {
    if (last.column == 0 && last.row == 0 && st.is_cell_empty(2, 2))
    {
      return { 2, 2 };
    }
    if (last.column == 0 && last.row == 2 && st.is_cell_empty(2, 0))
    {
      return { 2, 0 };
    }
    if (last.column == 2 && last.row == 2 && st.is_cell_empty(0, 0))
    {
      return { 0, 0 };
    }
    if (last.column == 2 && last.row == 0 && st.is_cell_empty(0, 2))
    {
      return { 0, 2 };
    }
    return INVALID_LOCATION;
  }

  std::vector<cell_location> strategy::find_empty_locations(const details::state& st) const {
    if (st.is_board_empty())
    {
      // This is every cell.
      return { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 }, { 2, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } };
    }
    auto res = std::vector<cell_location>{ };
    auto column = std::size_t{ 0 };
    auto row = std::size_t{ 0 };
    // Retrieve the board bits from the state.
    auto board = st.board();
    // For each of the 9 cells
    for (auto i = 0; i < 9; ++i)
    {
      // If the cell is empty, push it.
      if (!(board & details::state::ALL_CELL_BITS))
      {
        res.push_back({ column, row });
      }
      // Move to the next cell.
      ++column;
      if (column > 2)
      {
        ++row;
        column = 0;
      }
      // Shift the current cell off the board.
      board >>= 2;
    }
    return res;
  }

  bool strategy::would_win(const details::state& st, const cell_location& next) const {
    auto next_state = st;
    next_state.cell(next.column, next.row, cell_contents::o);
    return next_state.is_column_o(next.column) || next_state.is_row_o(next.row) || next_state.is_left_diagonal_o() ||
           next_state.is_right_diagonal_o();
  }

  bool strategy::would_lose(const details::state& st, const cell_location& next) const {
    auto next_state = st;
    next_state.cell(next.column, next.row, cell_contents::x);
    return next_state.is_column_x(next.column) || next_state.is_row_x(next.row) || next_state.is_left_diagonal_x() ||
           next_state.is_right_diagonal_x();
  }

  cell_location strategy::find_win(const details::state& st) const {
    if (st.count_o() > 1)
    {
      for (const auto& empty : find_empty_locations(st))
      {
        if (would_win(st, empty))
        {
          return empty;
        }
      }
    }
    return INVALID_LOCATION;
  }

  std::vector<cell_location> strategy::find_all_blocks(const details::state& st) const {
    auto res = std::vector<cell_location>{ };
    if (st.count_x() > 2)
    {
      for (const auto& empty : find_empty_locations(st))
      {
        if (would_lose(st, empty))
        {
          res.push_back(empty);
        }
      }
    }
    return res;
  }

  cell_location strategy::find_block(const details::state& st) const {
    if (st.count_x() > 1)
    {
      for (const auto& empty : find_empty_locations(st))
      {
        if (would_lose(st, empty))
        {
          return empty;
        }
      }
    }
    return INVALID_LOCATION;
  }
  cell_location strategy::find_fork_block(const details::state& st) const {
    // This bears some explanation.
    // A fork is a position originating on turn 3 or turn 6 where a mistake by
    // O allows X to win. In a fork position X can win in two different ways
    // which prevents a block by O. Such a position arises two turns after the
    // mistake by O and therefore requires additional foresight to prevent.
    // A position is excluded from being a fork if O can win before X in the
    // fork position.
    //
    // I think the complexity of this approach is something like O(n^4). Where n is the number of empty cells.
    // Obviously, that's not ideal but this should only run twice at most.
    auto available = std::vector<cell_location>{ };
    for (const auto& empty : find_empty_locations(st))
    {
      auto next = st;
      next.cell(empty.column, empty.row, cell_contents::o);
      auto fork = false;
      for (const auto& next_empty : find_empty_locations(next))
      {
        auto next_next = next;
        next_next.cell(next_empty.column, next_empty.row, cell_contents::x);
        auto blocks = find_all_blocks(next_next);
        // A position is only a fork if O would need to block more than one win by X AND O cannot win on the next turn.
        if (blocks.size() > 1 && !is_valid(find_win(next_next)))
        {
          fork = true;
        }
      }
      if (!fork)
      {
        available.push_back(empty);
      }
    }
    if (available.empty())
    {
      return INVALID_LOCATION;
    }
    auto selector = std::uniform_int_distribution<std::size_t>{ 0, available.size() - 1 };
    return available[selector(m_prng)];
  }

  strategy::strategy() : m_prng{ std::random_device{ }() } { }

  cell_location strategy::operator()(const game& g, const cell_location& last) const {
    return run(g, last);
  }

  cell_location strategy::operator()(const details::state& st, const cell_location& last) const {
    return run(st, last);
  }

  cell_location strategy::run(const game& g, const cell_location& last) const {
    return run(g.state(), last);
  }

  cell_location strategy::run(const details::state& st, const cell_location& last) const {
    // Step 1: Win the game.
    if (auto res = find_win(st); is_valid(res))
    {
      return res;
    }
    // Step 2: Block a win.
    if (auto res = find_block(st); is_valid(res))
    {
      return res;
    }
    if (st.filled_cells() == 3 || st.filled_cells() == 6)
    {
      // Step 3: Block forks.
      if (auto res = find_fork_block(st); is_valid(res))
      {
        return res;
      }
    }
    // Step 4: Play the center.
    if (st.is_cell_empty(1, 1))
    {
      return { 1, 1 };
    }
    // Step 5: Play an opposite corner.
    if (auto res = find_opposite_corner(st, last); is_valid(res))
    {
      return res;
    }
    // Step 6: Play an empty corner.
    if (auto res = find_corner(st); is_valid(res))
    {
      return res;
    }
    // Step 7: Play an empty edge.
    if (auto res = find_edge(st); is_valid(res))
    {
      return res;
    }
    throw std::runtime_error{ "A valid move could not be located." };
  }

}
