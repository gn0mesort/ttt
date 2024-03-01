#include <cassert>

#include "megatech/ttt/file_lock.hpp"

constexpr const char* LOCK_NAME{ "x" };

void test_basic_locking_1() {
  auto l = megatech::ttt::lockfile{ LOCK_NAME };
  assert(l.try_lock() == true);
  assert(l.try_lock() == false);
}

void test_basic_locking_2() {
  auto l = megatech::ttt::lockfile{ LOCK_NAME };
  auto l2 = megatech::ttt::lockfile{ LOCK_NAME };
  assert(l.try_lock() == true);
  assert(l2.try_lock() == false);
  l.unlock();
  assert(l2.try_lock() == true);
  assert(l.try_lock() == false);
}

void test_scoped_locking_1() {
  auto l = megatech::ttt::lockfile{ LOCK_NAME };
  {
    auto sfl = megatech::ttt::scoped_file_lock{ l };
    assert(l.try_lock() == false);
  }
  assert(l.try_lock() == true);
}

void test_scoped_locking_2() {
  auto l = megatech::ttt::lockfile{ LOCK_NAME };
  {
    auto sfl = megatech::ttt::scoped_file_lock{ l };
    try
    {
      auto sfl2 = megatech::ttt::scoped_file_lock{ l };
      assert(false);
    }
    catch (...) { }
    assert(l.try_lock() == false);
  }
}

void test_scoped_locking_3() {
  auto l = megatech::ttt::lockfile{ LOCK_NAME };
  auto l2 = megatech::ttt::lockfile{ LOCK_NAME };
  {
    auto sfl = megatech::ttt::scoped_file_lock{ l };
    try
    {
      auto sfl2 = megatech::ttt::scoped_file_lock{ l2 };
      assert(false);
    }
    catch (...) { }
    assert(l.try_lock() == false);
    assert(l2.try_lock() == false);
  }
}

int main() {
  test_basic_locking_1();
  test_basic_locking_2();
  test_scoped_locking_1();
  test_scoped_locking_2();
  test_scoped_locking_3();
  return 0;
}
