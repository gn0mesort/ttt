/**
 * @file interpreter.cpp
 * @brief Embedded language interpreter test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>

#include <string>

#include <megatech/ttt/details/interpreter.hpp>

void test_bad_chars() {
  auto program = std::string{ "!!" };
  auto interp = megatech::ttt::details::interpreter{ };
  auto output = interp.execute(program);
  assert(output.empty());
}


void test_interpreter() {
  auto program = std::string{ "++++++++[>+++++++++>+++++++++++++>++++++>++++<<<<-]>.>---.+++++++..+++.>----.>.<<+++++"
                              "+++.--------.+++.------.--------.>>+." };
  auto interp = megatech::ttt::details::interpreter{ };
  auto output = interp.execute(program);
  output.push_back('\0');
  auto result = std::string{ output.data() };
  assert(result == std::string{ "Hello, world!" });
}

void test_input() {
  auto program = std::string{ ",[->+<]>.>,." };
  auto input = std::vector<unsigned char>{ 48 };
  auto interp = megatech::ttt::details::interpreter{ };
  auto output = interp.execute(program, input);
  assert(output[0] == 48);
  assert(output[1] == 0);
}

void test_wrap_around() {
  auto program = std::string{ ">,>>>.,<<<." };
  auto input = std::vector<unsigned char>{ 0xff, 0xee };
  auto interp = megatech::ttt::details::interpreter{ 3 };
  auto output = interp.execute(program, input);
  assert(static_cast<unsigned char>(output[0]) == 0xff);
  assert(static_cast<unsigned char>(output[1]) == 0xee);
}

int main() {
  test_bad_chars();
  test_interpreter();
  test_input();
  test_wrap_around();
  return 0;
}
