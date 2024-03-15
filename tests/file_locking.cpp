/**
 * @file file_locking.cpp
 * @brief File locking test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>

#include <megatech/ttt/details/lockfile.hpp>

constexpr const char* LOCK_NAME{ "x" };

// Test lockfile lock/relock behavior.
void test_basic_locking_1() {
  auto l = megatech::ttt::details::lockfile{ LOCK_NAME };
  assert(l.try_lock() == true);
  assert(l.try_lock() == false);
}

// Test that two lockfiles cannot lock the same file.
void test_basic_locking_2() {
  auto l = megatech::ttt::details::lockfile{ LOCK_NAME };
  auto l2 = megatech::ttt::details::lockfile{ LOCK_NAME };
  assert(l.try_lock() == true);
  assert(l2.try_lock() == false);
  l.unlock();
  assert(l2.try_lock() == true);
  assert(l.try_lock() == false);
}

int main() {
  test_basic_locking_1();
  test_basic_locking_2();
  return 0;
}
