#include <cassert>

#include <filesystem>

#include <megatech/ttt/game.hpp>

#define GAME_FILE ".ttt"

void test_turn_alternation() {
  auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
  assert(g.get_play_state() == megatech::ttt::play_state::turn_x);
  g.take_turn(0, 0);
  assert(g.get_play_state() == megatech::ttt::play_state::turn_o);
  g.take_turn(1, 0);
  assert(g.get_play_state() == megatech::ttt::play_state::turn_x);
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
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 1);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(1, 0);
    g.take_turn(2, 1);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 2);
    g.take_turn(0, 0);
    g.take_turn(1, 2);
    g.take_turn(1, 0);
    g.take_turn(2, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  // Columns
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 1);
    g.take_turn(0, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(1, 0);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(1, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(2, 0);
    g.take_turn(0, 0);
    g.take_turn(2, 1);
    g.take_turn(0, 1);
    g.take_turn(2, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  // Diagonals
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(2, 0);
    g.take_turn(2, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(2, 0);
    g.take_turn(0, 0);
    g.take_turn(1, 1);
    g.take_turn(1, 0);
    g.take_turn(0, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_x);
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
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 2);
    g.take_turn(2, 1);
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(0, 2);
    g.take_turn(1, 0);
    g.take_turn(1, 2);
    g.take_turn(1, 1);
    g.take_turn(2, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
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
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(1, 0);
    g.take_turn(0, 1);
    g.take_turn(1, 1);
    g.take_turn(2, 0);
    g.take_turn(1, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(2, 0);
    g.take_turn(0, 1);
    g.take_turn(2, 1);
    g.take_turn(1, 1);
    g.take_turn(2, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
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
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE, megatech::ttt::game_mode::multiplayer };
    g.take_turn(0, 0);
    g.take_turn(2, 0);
    g.take_turn(1, 0);
    g.take_turn(1, 1);
    g.take_turn(0, 1);
    g.take_turn(0, 2);
    assert(g.get_play_state() == megatech::ttt::play_state::win_o);
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
  assert(g.get_play_state() == megatech::ttt::play_state::draw);
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
