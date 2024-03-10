#include <cassert>
#include <cinttypes>

#include <string>

#include <megatech/ttt/enums.hpp>

void test_to_string() {
  assert(megatech::ttt::to_string(megatech::ttt::game_mode::single_player) == std::string{ "single" });
  assert(megatech::ttt::to_string(megatech::ttt::game_mode::multiplayer) == std::string{ "multiplayer" });
  assert(megatech::ttt::to_string(megatech::ttt::cell_contents::empty) == std::string{ " " });
  assert(megatech::ttt::to_string(megatech::ttt::cell_contents::x) == std::string{ "X" });
  assert(megatech::ttt::to_string(megatech::ttt::cell_contents::o) == std::string{ "O" });
}

void test_to_game_mode() {
  assert(megatech::ttt::to_game_mode("single") == megatech::ttt::game_mode::single_player);
  assert(megatech::ttt::to_game_mode("multiplayer") == megatech::ttt::game_mode::multiplayer);
}

void test_bad_enums() {
  auto bad_mode = static_cast<megatech::ttt::game_mode>(12);
  try
  {
    megatech::ttt::to_string(bad_mode);
    assert(false);
  }
  catch (...) { }
  auto bad_contents = static_cast<megatech::ttt::cell_contents>(12);
  try
  {
    megatech::ttt::to_string(bad_contents);
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
  test_bad_strings();
  return 0;
}
