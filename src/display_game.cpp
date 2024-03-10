#include <iostream>
#include <exception>
#include <filesystem>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/utility.hpp>

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
    std::cerr << "An error occurred:" << std::endl << "\t\"" << err.what() << "\"" << std::endl;
    return 1;
  }
  return 0;
}
