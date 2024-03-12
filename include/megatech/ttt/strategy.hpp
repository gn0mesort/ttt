#ifndef MEGATECH_TTT_STRATEGY_HPP
#define MEGATECH_TTT_STRATEGY_HPP

#include <cstddef>

#include <random>
#include <limits>
#include <vector>

#include "details/state.hpp"

namespace megatech::ttt {

  class game;

  struct cell_location final {
    std::size_t column;
    std::size_t row;
  };

  class strategy final {
  private:
    static constexpr const cell_location INVALID_LOCATION{ std::numeric_limits<std::size_t>::max(),
                                                           std::numeric_limits<std::size_t>::max() };

    mutable std::default_random_engine m_prng{ };

    cell_location find_edge(const details::state& st) const;
    cell_location find_corner(const details::state& st) const;
    cell_location find_opposite_corner(const details::state& st, const cell_location& last) const;
    std::vector<cell_location> find_empty_locations(const details::state& st) const;
    bool would_win(const details::state& st, const cell_location& next) const;
    bool would_lose(const details::state& st, const cell_location& next) const;
    cell_location find_win(const details::state& st) const;
    std::vector<cell_location> find_all_blocks(const details::state& st) const;
    cell_location find_block(const details::state& st) const;
    cell_location find_fork_block(const details::state& st) const;
  public:
    strategy();
    strategy(const strategy& other) = default;
    strategy(strategy&& other) = default;

    ~strategy() noexcept = default;

    strategy& operator=(const strategy& rhs) = default;
    strategy& operator=(strategy&& rhs) = default;
    cell_location operator()(const game& g, const cell_location& last) const;
    cell_location operator()(const details::state& st, const cell_location& last) const;

    cell_location run(const game& g, const cell_location& last) const;
    cell_location run(const details::state& st, const cell_location& last) const;
  };

}

#endif
