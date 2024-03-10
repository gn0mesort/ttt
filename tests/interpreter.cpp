#include <cassert>

#include <string>

#include <megatech/ttt/details/interpreter.hpp>

void test_interpreter() {
  auto program = std::string{ "++++++++[>+++++++++>+++++++++++++>++++++>++++<<<<-]>.>---.+++++++..+++.>----.>.<<+++++"
                              "+++.--------.+++.------.--------.>>+." };
  auto interp = megatech::ttt::details::interpreter{ };
  auto output = interp.execute(program);
  output.push_back('\0');
  auto result = std::string{ output.data() };
  assert(result == std::string{ "Hello, world!" });
}

int main() {
  test_interpreter();
  return 0;
}
