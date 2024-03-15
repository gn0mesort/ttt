/**
 * @file display_game.cpp
 * @brief Game state display application.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include <iostream>
#include <exception>
#include <filesystem>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/utility.hpp>

void display_help(const std::string& name, const std::string& message) {
  std::cerr << message << std::endl;
  std::cerr << "USAGE: " << name << std::endl;
}

int main(int argc, char** argv) {
  try
  {
    if (auto res = megatech::ttt::initialize(argc, argv); res)
    {
      return res;
    }
    auto game_path = megatech::ttt::find_home_directory() / megatech::ttt::DEFAULT_GAME_NAME;
    auto g = megatech::ttt::game{ game_path };
    std::cout << g << std::endl;
  }
  catch (const std::exception& err)
  {
    display_help(argv[0], err.what());
    return 1;
  }
  return 0;
}
