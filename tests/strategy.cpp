/**
 * @file strategy.cpp
 * @brief Single player game strategy test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>

#include <megatech/ttt/enums.hpp>
#include <megatech/ttt/strategy.hpp>
#include <megatech/ttt/details/state.hpp>

megatech::ttt::details::state initialized_state() {
  auto st = megatech::ttt::details::state{ };
  st.mode(megatech::ttt::game_mode::single_player);
  return st;
}

bool is_corner(const megatech::ttt::cell_location& location) {
  return location.column != 1 && location.row != 1;
}

bool is_center(const megatech::ttt::cell_location& location) {
  return location.column == 1 && location.row == 1;
}

void test_openings() {
  auto st = initialized_state();
  st.cell(0, 0, megatech::ttt::cell_contents::x);
  auto strat = megatech::ttt::strategy{ };
  auto res = strat(st, { 0, 0 });
  assert(is_center(res));
  st = initialized_state();
  st.cell(1, 1, megatech::ttt::cell_contents::x);
  res = strat(st, { 1, 1 });
  assert(is_corner(res));
  st = initialized_state();
  st.cell(1, 0, megatech::ttt::cell_contents::x);
  res = strat(st, { 1, 0 });
  assert(is_center(res) || is_corner(res));
}

void test_win() {
  // Win on a row
  auto st = initialized_state();
  st.cell(1, 1, megatech::ttt::cell_contents::x);
  st.cell(0, 0, megatech::ttt::cell_contents::o);
  st.cell(2, 2, megatech::ttt::cell_contents::x);
  st.cell(1, 0, megatech::ttt::cell_contents::o);
  st.cell(1, 2, megatech::ttt::cell_contents::x);
  auto strat = megatech::ttt::strategy{ };
  auto res = strat(st, { 1, 2 });
  assert(res.column == 2 && res.row == 0);
  // Win on a column
  st = initialized_state();
  st.cell(1, 1, megatech::ttt::cell_contents::x);
  st.cell(0, 2, megatech::ttt::cell_contents::o);
  st.cell(2, 1, megatech::ttt::cell_contents::x);
  st.cell(0, 1, megatech::ttt::cell_contents::o);
  st.cell(2, 0, megatech::ttt::cell_contents::x);
  res = strat(st, { 2, 0 });
  assert(res.column == 0 && res.row == 0);
  // Win diagonally
  st = initialized_state();
  st.cell(0, 0, megatech::ttt::cell_contents::x);
  st.cell(1, 1, megatech::ttt::cell_contents::o);
  st.cell(1, 0, megatech::ttt::cell_contents::x);
  st.cell(2, 0, megatech::ttt::cell_contents::o);
  st.cell(2, 2, megatech::ttt::cell_contents::x);
  res = strat(st, { 2, 2 });
  assert(res.column == 0 && res.row == 2);
}

void test_block() {
  // Block on a row
  auto st = initialized_state();
  st.cell(0, 0, megatech::ttt::cell_contents::x);
  st.cell(1, 1, megatech::ttt::cell_contents::o);
  st.cell(1, 0, megatech::ttt::cell_contents::x);
  auto strat = megatech::ttt::strategy{ };
  auto res = strat(st, { 1, 0 });
  assert(res.column == 2 && res.row == 0);
  // Block on a column
  st = initialized_state();
  st.cell(0, 0, megatech::ttt::cell_contents::x);
  st.cell(1, 1, megatech::ttt::cell_contents::o);
  st.cell(0, 1, megatech::ttt::cell_contents::x);
  res = strat(st, { 0, 1 });
  assert(res.column == 0 && res.row == 2);
  // Block diagonally
  st = initialized_state();
  st.cell(1, 1, megatech::ttt::cell_contents::x);
  st.cell(0, 0, megatech::ttt::cell_contents::o);
  st.cell(2, 0, megatech::ttt::cell_contents::x);
  res = strat(st, { 2, 0 });
  assert(res.column == 0 && res.row == 2);
}

void test_fork_block() {
  auto st = initialized_state();
  st.cell(0, 0, megatech::ttt::cell_contents::x);
  st.cell(1, 1, megatech::ttt::cell_contents::o);
  st.cell(2, 2, megatech::ttt::cell_contents::x);
  auto strat = megatech::ttt::strategy{ };
  auto res = strat(st, { 2, 2 });
  assert(!is_corner(res));
}

int main() {
  test_openings();
  test_win();
  test_block();
  test_fork_block();
  return 0;
}
