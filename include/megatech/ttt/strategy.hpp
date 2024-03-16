/**
 * @file strategy.hpp
 * @brief Tic-Tac-Toe CPU player strategy objects.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_STRATEGY_HPP
#define MEGATECH_TTT_STRATEGY_HPP

#include <cstddef>

#include <random>
#include <limits>
#include <vector>

#include "details/state.hpp"

namespace megatech::ttt {

  class game;

  /**
   * @brief A utility structure for that indicates (column, row) location on a Tic-Tac-Toe board.
   * @details Indices out of bounds for Tic-Tac-Toe (i.e., greater than 2) indicate an invalid state.
   */
  struct cell_location final {
    /**
     * @brief The column index.
     */
    std::size_t column;

    /**
     * @brief The row index.
     */
    std::size_t row;
  };

  /**
   * @brief An object representing a strategy for the second player (O) of a Tic-Tac-Toe game.
   */
  class strategy final {
  private:
    static constexpr cell_location INVALID_LOCATION{ std::numeric_limits<std::size_t>::max(),
                                                     std::numeric_limits<std::size_t>::max() };

    // Play selection CAN modify the state of this object, but the PRNG state is never exposed in the interface of
    // strategy.
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
    /**
     * @brief Create a new strategy object.
     */
    strategy();

    /**
     * @brief Copy a strategy object from another.
     * @param other The strategy to copy.
     */
    strategy(const strategy& other) = default;

    /**
     * @brief Move a strategy object from another.
     * @param other The strategy to move data from.
     */
    strategy(strategy&& other) = default;

    /**
     * @brief Destroy a strategy.
     */
    ~strategy() noexcept = default;

    /**
     * @brief Copy a strategy object from another.
     * @param rhs The strategy to copy.
     * @return A reference to the copied to strategy.
     */
    strategy& operator=(const strategy& rhs) = default;

    /**
     * @brief Move a strategy object from another.
     * @param rhs The strategy to move data from.
     * @return A reference to the move to strategy.
     */
    strategy& operator=(strategy&& rhs) = default;

    /**
     * @brief Apply the strategy and choose a cell location to mark for O.
     * @param g The game to base the location on.
     * @param last The location of the last mark made by the X player.
     * @throw std::runtime_error If the strategy fails to find a valid location.
     */
    cell_location operator()(const game& g, const cell_location& last) const;

    /**
     * @brief Apply the strategy and choose a cell location to mark for O.
     * @param st The state to base the location on.
     * @param last The location of the last mark made by the X player.
     * @throw std::runtime_error If the strategy fails to find a valid location.
     */
    cell_location operator()(const details::state& st, const cell_location& last) const;

    /**
     * @brief Apply the strategy and choose a cell location to mark for O.
     * @param g The game to base the location on.
     * @param last The location of the last mark made by the X player.
     * @throw std::runtime_error If the strategy fails to find a valid location.
     */
    cell_location run(const game& g, const cell_location& last) const;

    /**
     * @brief Apply the strategy and choose a cell location to mark for O.
     * @param st The state to base the location on.
     * @param last The location of the last mark made by the X player.
     * @throw std::runtime_error If the strategy fails to find a valid location.
     */
    cell_location run(const details::state& st, const cell_location& last) const;
  };

}

#endif
