#include <stdexcept>
#include <filesystem>
#include <iostream>

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
    auto stat = std::filesystem::status(game_path);
    if (std::filesystem::exists(stat))
    {
      try
      {
        auto g = megatech::ttt::game{ game_path };
      }
      catch (const std::runtime_error& err)
      {
        std::cerr << "An error occurred when reading the game data file." << std::endl << "No action will be taken."
                  << std::endl;
        throw;
      }
      std::filesystem::remove_all(game_path);
      std::cerr << "The game data file @ " << game_path << " was deleted." << std::endl;
    }
    else
    {
      std::cerr << "No existing game file found @ " << game_path << "." << std::endl;
    }
  }
  catch (const std::exception& err)
  {
    std::cerr << "An error occurred:" << std::endl << "\t\"" << err.what() << "\"" << std::endl;
    return 1;
  }
  return 0;
}
