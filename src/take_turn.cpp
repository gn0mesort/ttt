#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <string>

#include <megatech/ttt/game.hpp>
#include <megatech/ttt/strategy.hpp>
#include <megatech/ttt/utility.hpp>

void display_help(const std::string& name, const std::string& message) {
  std::cerr << message << std::endl;
  std::cerr << "USAGE: " << name << " COLUMN ROW" << std::endl;
  std::cerr << "\tValid values are \"0\", \"1\", or \"2\" for both columns and rows." << std::endl;
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
      throw std::runtime_error{ "Too few program arguments were provided." };
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
    if (g.state().phase() == megatech::ttt::game_phase::turn_o &&
        g.state().mode() == megatech::ttt::game_mode::single_player)
    {
      auto strat = megatech::ttt::strategy{ };
      auto location = strat(g, { column, row });
      g.take_turn(location.column, location.row);
    }
    std::cout << g << std::endl;
  }
  catch (const std::exception& err)
  {
    display_help(argv[0], err.what());
    return 1;
  }
  return 0;
}
