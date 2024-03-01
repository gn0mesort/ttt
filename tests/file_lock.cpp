#include "megatech/ttt/file_lock.hpp"

int main() {
  auto l = megatech::ttt::lockfile{ "x" };
  {
    auto sfl = megatech::ttt::scoped_file_lock{ l };
    if (l.try_lock())
    {
      return 1;
    }
    try
    {
      auto sfl2 = megatech::ttt::scoped_file_lock{ l };
      return 1;
    }
    catch (const std::exception& ex) { }
    if (l.try_lock())
    {
      return 1;
    }
    auto l2 = megatech::ttt::lockfile{ "x" };
    try
    {
      auto sfl3 = megatech::ttt::scoped_file_lock{ l2 };
      return 1;
    }
    catch (const std::exception& ex) { }
    if (l2.try_lock())
    {
      return 1;
    }
  }
  return 0;
}
