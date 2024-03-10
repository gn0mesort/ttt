#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <string>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/utility.hpp>

void display_help(const std::string& name, const std::string& message) {
  std::cerr << message << std::endl;
  std::cerr << "USAGE: " << name << " COLUMN ROW " << std::endl;
}

int main(int argc, char** argv) {
  try
  {
    if (auto res = megatech::ttt::initialize(argc, argv); res)
    {
      return res;
    }
    if (argc < 3)
    {
      display_help(argv[0], "Too few program arguments were provided.");
    }
    auto column = std::size_t{ 0 };
    auto row = std::size_t{ 0 };
    {
      auto s_in = std::istringstream{ argv[1] };
      s_in >> column;
      if (s_in.fail())
      {
        throw std::runtime_error{ "The column value could not be read." };
      }
      s_in.str(argv[2]);
      s_in.seekg(0, std::ios::beg);
      s_in >> row;
      if (s_in.fail())
      {
        throw std::runtime_error{ "The row value could not be read." };
      }
    }
    auto game_path = megatech::ttt::find_home_directory() / megatech::ttt::DEFAULT_GAME_NAME;
    auto g = megatech::ttt::game{ game_path };
    g.take_turn(column, row);
    switch (g.state().mode())
    {
    case megatech::ttt::game_mode::single_player:
      // TODO: CPU Player takes a turn.
      break;
    default:
      break;
    }
    std::cout << g << std::endl;
  }
  catch (const std::exception& err)
  {
    std::cerr << "An error occurred:" << std::endl << "\t\"" << err.what() << "\"" << std::endl;
    return 1;
  }
  return 0;
}
