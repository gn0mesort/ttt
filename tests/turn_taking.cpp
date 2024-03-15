/**
 * @file turn_taking.cpp
 * @brief Game turn taking and state update test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>

#include <filesystem>

#include <megatech/ttt/game.hpp>

#define GAME_FILE ".ttt"

void test_turn_alternation() {
  auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
  assert(g.state().phase() == megatech::ttt::game_phase::turn_x);
  g.take_turn(0, 0);
  assert(g.state().phase() == megatech::ttt::game_phase::turn_o);
  g.take_turn(1, 0);
  assert(g.state().phase() == megatech::ttt::game_phase::turn_x);
}

void test_x_wins() {
  // Rows
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(2, 0);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 1);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(1, 0);
    g.take_turn(2, 1);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 2);
    g.take_turn(0, 0);
    g.take_turn(1, 2);
    g.take_turn(1, 0);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  // Columns
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 1);
    g.take_turn(0, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(1, 0);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(1, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(2, 0);
    g.take_turn(0, 0);
    g.take_turn(2, 1);
    g.take_turn(0, 1);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  // Diagonals
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(2, 0);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(2, 0);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(1, 0);
    g.take_turn(0, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_x);
  }
  std::filesystem::remove_all(GAME_FILE);
}

void test_o_wins() {
  // Rows
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 1);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(1, 0);
    g.take_turn(0, 2);
    g.take_turn(2, 0);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 2);
    g.take_turn(2, 1);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(0, 2);
    g.take_turn(1, 0);
    g.take_turn(1, 2);
    g.take_turn(1, 1);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  // Columns
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(1, 0);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(2, 0);
    g.take_turn(0, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 1);
    g.take_turn(2, 0);
    g.take_turn(1, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(2, 0);
    g.take_turn(0, 1);
    g.take_turn(2, 1);
    g.take_turn(1, 1);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  // Diagonals
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(1, 0);
    g.take_turn(0, 0);
    g.take_turn(2, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(2, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(2, 0);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(0, 2);
    assert(g.state().phase() == megatech::ttt::game_phase::win_o);
  }
  std::filesystem::remove_all(GAME_FILE);
}

void test_draw() {
  auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
  // X O X
  // O X X
  // O X O
  g.take_turn(0, 0);
  g.take_turn(1, 0);
  g.take_turn(2, 0);
  g.take_turn(0, 1);
  g.take_turn(1, 1);
  g.take_turn(0, 2);
  g.take_turn(2, 1);
  g.take_turn(2, 2);
  g.take_turn(1, 2);
  assert(g.state().phase() == megatech::ttt::game_phase::draw);
  std::filesystem::remove_all(GAME_FILE);
}

int main() {
  try
  {
    test_turn_alternation();
    test_x_wins();
    test_o_wins();
    test_draw();
  }
  catch (...)
  {
    std::filesystem::remove_all(GAME_FILE);
  }
  return 0;
}
