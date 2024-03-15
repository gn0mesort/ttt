/**
 * @file frobnication.cpp
 * @brief Memory obfuscation test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>

#include <string>
#include <array>

#include <megatech/ttt/utility.hpp>

void test_key() {
  auto arr = std::array<unsigned char, 1>{ 0 };
  megatech::ttt::details::frobnicate(arr);
  assert(arr[0] == 42);
  megatech::ttt::details::frobnicate(arr);
  assert(arr[0] == 0);
}

void test_frobnicate() {
  auto str = std::string{ "Hello, world!" };
  megatech::ttt::details::frobnicate(str);
  assert(str != std::string{ "Hello, world!" });
  megatech::ttt::details::frobnicate(str);
  assert(str == std::string{ "Hello, world!" });
}

int main() {
  test_key();
  test_frobnicate();
  return 0;
}
