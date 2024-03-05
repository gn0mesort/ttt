#include <cassert>
#include <cstdlib>

#include <filesystem>
#include <iostream>

#include <megatech/ttt/utility.hpp>

void test_find_home_directory() {
  auto desired = std::filesystem::path{ };
  {
    auto home = std::getenv("RESULT_HOME");
    assert(home);
    desired = std::filesystem::absolute(home);
  }
  assert(megatech::ttt::find_home_directory() == desired);
}

int main() {
  test_find_home_directory();
  return 0;
}

