#include <cctype>

#include <stdexcept>
#include <filesystem>
#include <iostream>
#include <string>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/utility.hpp>

int main(int argc, char** argv) {
  if (argc < 2)
  {
    std::cerr << "Too few program arguments were found. Please run either:" << std::endl
              << "\t" << argv[0] << " single" << std::endl
              << "\t" << argv[0] << " multiplayer" << std::endl
              << "for a single player or multiplayer game respectively." << std::endl;
    return 1;
  }
  auto g = static_cast<megatech::ttt::game*>(nullptr);
  try
  {
    auto mode = megatech::ttt::game_mode::single_player;
    {
      auto mode_str = std::string{ argv[1] };
      for (auto& c : mode_str)
      {
        c = std::tolower(c);
      }
      mode = megatech::ttt::to_game_mode(mode_str);
    }
    auto home_dir = megatech::ttt::find_home_directory();
    std::cerr << "A home directory was found @ " << home_dir << "." << std::endl;
    auto game_path = home_dir / megatech::ttt::DEFAULT_GAME_NAME;
    auto stat = std::filesystem::status(game_path);
    if (std::filesystem::exists(stat))
    {
      std::cerr << "An existing game file was found @ " << game_path
                << ". An attempt will be made to clear the file." << std::endl;
      g = new megatech::ttt::game{ game_path };
      g->clear();
    }
    else
    {
      std::cerr << "No existing game file found @ " << game_path << ". Creating a new game." << std::endl;
      g = new megatech::ttt::game{ game_path };
    }
    g->set_mode(mode);
    delete g;
    std::cerr << "A new game was successfully created!" << std::endl;
  }
  catch (const std::exception& err)
  {
    std::cerr << "An error occurred:" << std::endl << "\t\"" << err.what() << "\"" << std::endl;
    if (g)
    {
      delete g;
    }
    return 1;
  }
  return 0;
}
