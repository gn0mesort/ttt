#ifndef MEGATECH_TTT_GAME_HPP
#define MEGATECH_TTT_GAME_HPP

#include <cinttypes>

#include <filesystem>

#include "details/lockfile.hpp"

namespace megatech::ttt {

  class game final {
  private:
    std::uint32_t m_state{ };
    std::filesystem::path m_path{ };
    details::lockfile m_lock{ ".ttt" };
  public:
    game();
    game(const std::filesystem::path& path);

    ~game() noexcept;
  };

}

#endif
