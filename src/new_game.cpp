#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <string>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/utility.hpp>

int main(int argc, char** argv) {
  try
  {
    if (auto res = megatech::ttt::initialize(argc, argv); res)
    {
      return res;
    }
    auto mode = megatech::ttt::game_mode::single_player;
    if (argc >= 2)
    {
      auto mode_str = std::string{ argv[1] };
      mode = megatech::ttt::to_game_mode(megatech::ttt::tolower(mode_str));
    }
    auto home_dir = megatech::ttt::find_home_directory();
    auto game_path = home_dir / megatech::ttt::DEFAULT_GAME_NAME;
    auto stat = std::filesystem::status(game_path);
    if (std::filesystem::exists(stat))
    {
      std::cerr << "An existing game file was found @ " << game_path
                << ". An attempt will be made to clear the file." << std::endl;
    }
    else
    {
      std::cerr << "No existing game file found @ " << game_path << ". Creating a new game." << std::endl;
    }
    {
      auto g = megatech::ttt::game{ game_path, mode };
      std::cout << g << std::endl;
    }
  }
  catch (const std::exception& err)
  {
    std::cerr << "An error occurred:" << std::endl << "\t\"" << err.what() << "\"" << std::endl;
    return 1;
  }
  return 0;
}
