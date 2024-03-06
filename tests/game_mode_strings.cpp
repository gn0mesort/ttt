#include <cassert>
#include <cinttypes>

#include <string>

#include <megatech/ttt/game.hpp>

void test_to_string() {
  assert(megatech::ttt::to_string(megatech::ttt::game_mode::single_player) == std::string{ "single" });
  assert(megatech::ttt::to_string(megatech::ttt::game_mode::multiplayer) == std::string{ "multiplayer" });
}

void test_to_game_mode() {
  assert(megatech::ttt::to_game_mode("single") == megatech::ttt::game_mode::single_player);
  assert(megatech::ttt::to_game_mode("multiplayer") == megatech::ttt::game_mode::multiplayer);
}

void test_bad_enums() {
  auto bad = static_cast<megatech::ttt::game_mode>(12);
  try
  {
    megatech::ttt::to_string(bad);
    assert(false);
  }
  catch (...) { }
}

void test_bad_strings() {
  try
  {
    megatech::ttt::to_game_mode("not a real mode");
    assert(false);
  }
  catch (...) { }
}

int main() {
  test_to_string();
  test_to_game_mode();
  test_bad_enums();
  test_to_game_mode();
  return 0;
}
