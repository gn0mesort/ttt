#ifndef MEGATECH_TTT_GAME_HPP
#define MEGATECH_TTT_GAME_HPP

#include <cinttypes>

#include <string>
#include <filesystem>

#include "details/lockfile.hpp"

namespace megatech::ttt {

  enum class game_mode : std::uint32_t {
    single_player = 0,
    multiplayer = 0x80'00'00'00
  };

  std::string to_string(const game_mode mode);
  game_mode to_game_mode(const std::string& name);

  const std::filesystem::path DEFAULT_GAME_NAME{ ".ttt" };

  class game final {
  private:
    static constexpr const std::uint32_t MULTIPLAYER_BIT{ 0x80'00'00'00 };

    std::uint32_t m_state{ };
    std::filesystem::path m_path{ };
    details::lockfile m_lock{ ".ttt" };
  public:
    game(const std::filesystem::path& path);
    game(const game& other) = delete;
    game(game&& other) = default;

    ~game() noexcept;

    game& operator=(const game& rhs) = delete;
    game& operator=(game&& rhs) = delete;

    void clear();
    game_mode get_mode() const;
    void set_mode(const game_mode mode);
  };

}

#endif
