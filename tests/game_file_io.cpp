/**
 * @file game_file_io.cpp
 * @brief Game data file I/O test.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <cassert>
#include <cstring>
#include <cstddef>

#include <filesystem>
#include <fstream>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/details/data_file.hpp>

#define GAME_FILE_NAME ".ttt"
#define LOCK_FILE_NAME ".~lock.ttt"

void test_new_game_read_write() {
  std::filesystem::remove_all(GAME_FILE_NAME);
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME, megatech::ttt::game_mode::single_player };
  }
  auto f_in = std::ifstream{ GAME_FILE_NAME, std::ios::binary | std::ios::ate };
  assert(static_cast<std::size_t>(f_in.tellg()) > sizeof(megatech::ttt::details::data_file_header));
  f_in.seekg(0, std::ios::beg);
  auto header = megatech::ttt::details::data_file_header{ };
  f_in.read(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
  assert(std::memcmp(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                     megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH) == 0);
  assert(header.version == megatech::ttt::details::DATA_FILE_VERSION_1);
  auto body = megatech::ttt::details::data_file_body_v1{ };
  f_in.read(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  assert(body.endianness == megatech::ttt::details::DATA_FILE_CORRECT_ENDIANNESS);
  assert(body.state == 0);
  std::filesystem::remove_all(GAME_FILE_NAME);
}

void test_irregular_file() {
  std::filesystem::remove_all(GAME_FILE_NAME);
  std::filesystem::create_directory(GAME_FILE_NAME);
  try
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME };
    assert(false);
  }
  catch (...) { }
  std::filesystem::remove_all(GAME_FILE_NAME);
}

void test_existing_file_read_write() {
  // This is a crafted value that will pass validation but is otherwise a meaningless state.
  constexpr const auto STATE_VALUE = 0x90'01'55'55;
  std::filesystem::remove_all(GAME_FILE_NAME);
  {
    auto f_out = std::ofstream{ GAME_FILE_NAME, std::ios::binary | std::ios::trunc };
    auto header = megatech::ttt::details::data_file_header{ };
    std::memcpy(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH);
    header.version = megatech::ttt::details::DATA_FILE_VERSION_1;
    f_out.write(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
    auto body = megatech::ttt::details::data_file_body_v1{ };
    body.endianness = megatech::ttt::details::DATA_FILE_CORRECT_ENDIANNESS;
    body.state = STATE_VALUE;
    f_out.write(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME };
  }
  auto f_in = std::ifstream{ GAME_FILE_NAME, std::ios::binary | std::ios::ate };
  assert(static_cast<std::size_t>(f_in.tellg()) > sizeof(megatech::ttt::details::data_file_header));
  f_in.seekg(0, std::ios::beg);
  auto header = megatech::ttt::details::data_file_header{ };
  f_in.read(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
  assert(std::memcmp(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                     megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH) == 0);
  assert(header.version == megatech::ttt::details::DATA_FILE_VERSION_1);
  auto body = megatech::ttt::details::data_file_body_v1{ };
  f_in.read(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  assert(body.endianness == megatech::ttt::details::DATA_FILE_CORRECT_ENDIANNESS);
  assert(body.state == STATE_VALUE);
  std::filesystem::remove_all(GAME_FILE_NAME);
}

void test_corrupt_file_header() {
  std::filesystem::remove_all(GAME_FILE_NAME);
  {
    auto f_out = std::ofstream{ GAME_FILE_NAME, std::ios::binary | std::ios::trunc };
    auto header = megatech::ttt::details::data_file_header{ };
    std::memset(header.magic, 0xff, megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH);
    header.version = 0xff;
    f_out.write(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
    auto body = megatech::ttt::details::data_file_body_v1{ };
    body.endianness = megatech::ttt::details::DATA_FILE_CORRECT_ENDIANNESS;
    body.state = 0;
    f_out.write(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  }
  try
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME };
    assert(false);
  }
  catch (...) { }
  std::filesystem::remove_all(GAME_FILE_NAME);
}

void test_corrupt_file_body() {
  std::filesystem::remove_all(GAME_FILE_NAME);
  {
    auto f_out = std::ofstream{ GAME_FILE_NAME, std::ios::binary | std::ios::trunc };
    auto header = megatech::ttt::details::data_file_header{ };
    std::memcpy(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH);
    header.version = megatech::ttt::details::DATA_FILE_VERSION_1;
    f_out.write(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
    auto body = megatech::ttt::details::data_file_body_v1{ };
    body.endianness = 0;
    body.state = 0;
    f_out.write(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  }
  try
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME };
    assert(false);
  }
  catch (...) { }
  std::filesystem::remove_all(GAME_FILE_NAME);
}

void test_state_byteswapping() {
  // This is a crafted value that will pass validation but is otherwise a meaningless state.
  constexpr const auto STATE_VALUE = 0x90'01'55'55;
  constexpr const auto REVERSE_STATE_VALUE = 0x55'55'01'90;
  std::filesystem::remove_all(GAME_FILE_NAME);
  {
    auto f_out = std::ofstream{ GAME_FILE_NAME, std::ios::binary | std::ios::trunc };
    auto header = megatech::ttt::details::data_file_header{ };
    std::memcpy(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH);
    header.version = megatech::ttt::details::DATA_FILE_VERSION_1;
    f_out.write(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
    auto body = megatech::ttt::details::data_file_body_v1{ };
    body.endianness = megatech::ttt::details::DATA_FILE_REVERSE_ENDIANNESS;
    body.state = REVERSE_STATE_VALUE;
    f_out.write(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  }
  {
    auto g = megatech::ttt::game{ GAME_FILE_NAME };
  }
  auto f_in = std::ifstream{ GAME_FILE_NAME, std::ios::binary | std::ios::ate };
  assert(static_cast<std::size_t>(f_in.tellg()) > sizeof(megatech::ttt::details::data_file_header));
  f_in.seekg(0, std::ios::beg);
  auto header = megatech::ttt::details::data_file_header{ };
  f_in.read(reinterpret_cast<char*>(&header), sizeof(megatech::ttt::details::data_file_header));
  assert(std::memcmp(header.magic, megatech::ttt::details::DATA_FILE_HEADER_MAGIC,
                     megatech::ttt::details::DATA_FILE_HEADER_MAGIC_LENGTH) == 0);
  assert(header.version == megatech::ttt::details::DATA_FILE_VERSION_1);
  auto body = megatech::ttt::details::data_file_body_v1{ };
  f_in.read(reinterpret_cast<char*>(&body), sizeof(megatech::ttt::details::data_file_body_v1));
  assert(body.endianness == megatech::ttt::details::DATA_FILE_CORRECT_ENDIANNESS);
  assert(body.state == STATE_VALUE);
  std::filesystem::remove_all(GAME_FILE_NAME);
}

int main() {
  try
  {
    test_new_game_read_write();
    test_irregular_file();
    test_existing_file_read_write();
    test_corrupt_file_header();
    test_state_byteswapping();
  }
  catch (...)
  {
    std::filesystem::remove_all(GAME_FILE_NAME);
    throw;
  }
  return 0;
}
