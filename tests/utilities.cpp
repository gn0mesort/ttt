#include <cassert>

#include <string>
#include <array>

#include <megatech/ttt/utility.hpp>

void test_tolower() {
  auto str = std::string{ "HELLO, WORLD!" };
  assert(megatech::ttt::tolower(str) == std::string{ "hello, world!" });
}

void test_initialize() {
  const auto args = std::array<const char*, 3>{ "/usr/bin/ttt-new-game", "multiplayer", nullptr };
  megatech::ttt::initialize(args.size() - 1, args.data());
  assert(std::locale{ } == std::locale{ "" });
}

int main() {
  test_tolower();
  test_initialize();
  return 0;
}
