/**
 * @file game.hpp
 * @brief Tic-Tac-Toe game state objects.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_GAME_HPP
#define MEGATECH_TTT_GAME_HPP

#include <cstddef>
#include <cinttypes>

#include <iosfwd>
#include <string>
#include <filesystem>

#include "enums.hpp"

#include "details/lockfile.hpp"
#include "details/state.hpp"

namespace megatech::ttt {

  /**
   * @brief The default game file name used by applications.
   */
  const std::filesystem::path DEFAULT_GAME_NAME{ ".ttt" };

  /**
   * @brief An object representing the state of a game of Tic-Tac-Toe.
   * @details The game object is responsible for enforcing the rules of Tic-Tac-Toe. It is also responsible for
   *          persisting that state between executions. When a game is created the corresponding file is read. When
   *          a game is destroyed the state is written back to the same file.
   */
  class game final {
  private:
    details::state m_state{ };
    std::filesystem::path m_path{ };
    details::lockfile m_lock{ DEFAULT_GAME_NAME };

    void read_data_file();
    cell_contents find_winner() const;
    void update_play_state();
    void take_turn(const std::size_t column, const std::size_t row, const cell_contents value);
  public:
    /**
     * @brief Create a game using the existing state in the given file.
     * @param path A path to a valid game data file.
     * @throw std::runtime_error If the input path is not valid, the file indicated by the path cannot be locked,
     *                           or the file data is corrupt.
     */
    explicit game(const std::filesystem::path& path);

    /**
     * @brief Create a game with a new state in the given file.
     * @details Unlike the other constructor this constructor always initializes the game's state. If the game file
     *          already exists, it will be read (to check it for validity) and then discarded.
     * @param path A path to a valid game data file.
     * @param mode The mode (e.g., single player or multiplayer) of the newly created game.
     * @throw std::runtime_error If the input path is not valid, the file indicated by the path cannot be locked,
     *                           or the file data is corrupt.
     */
    game(const std::filesystem::path& path, const game_mode mode);

    /// @cond
    game(const game& other) = delete;
    game(game&& other) = default;
    /// @endcond

    /**
     * @brief Destroy a game object.
     * @details During destruction the state of the game is always written back to storage.
     */
    ~game() noexcept;

    /// @cond
    game& operator=(const game& rhs) = delete;
    game& operator=(game&& rhs) = delete;
    /// @endcond

    /**
     * @brief Retrieve the game's internal state.
     * @details This is primarily useful for querying the current state of the game and duplicating it.
     * @return A reference to the game's state object.
     */
    const details::state& state() const;

    /**
     * @brief Take a turn by marking a cell with the current player's mark.
     * @details This is the main interface through which a game is played. If the indicated cell is unmarked, the game
     *          will mark the cell for the current player. Then the game will update its own state based on the usual
     *          rules of Tic-Tac-Toe.
     * @param column The column index of the cell to mark.
     * @param row The row index of the cell to mark.
     * @throw std::runtime_error If the indicated cell is already marked.
     */
    void take_turn(const std::size_t column, const std::size_t row);
  };

  /**
   * @brief Write a game object to an output stream.
   * @details This is a utility function for displaying a game's state. It will output the following:
   *            - A drawing of the game board with all appropriate marks and a legend
   *            - Whether the current game is single player or multiplayer
   *            - The current game phase
   *          The output is not terminated with a new line.
   * @param os The output stream to write to.
   * @param g The game to write out.
   * @return A reference to os.
   */
  template <typename CharT, typename Traits>
  std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const game& g) {
    const auto& st = g.state();
    os << "    0  1  2 " << std::endl
       << "  0  " << to_string(st.cell(0, 0)) << "|" << to_string(st.cell(1, 0)) << "|" << to_string(st.cell(2, 0))
       << "  " << std::endl << "    ------- " << std::endl
       << "  1  " << to_string(st.cell(0, 1)) << "|" << to_string(st.cell(1, 1)) << "|" << to_string(st.cell(2, 1))
       << "  " << std::endl << "    ------- " << std::endl
       << "  2  " << to_string(st.cell(0, 2)) << "|" << to_string(st.cell(1, 2)) << "|" << to_string(st.cell(2, 2))
       << "  "<< std::endl;
    switch (st.mode())
    {
    case game_mode::single_player:
      os << "This is a single player game." << std::endl;
      break;
    case game_mode::multiplayer:
      os << "This is a multiplayer game." << std::endl;
      break;
    default:
      break;
    }
    switch (st.phase())
    {
    case game_phase::turn_x:
      os << "It is X's turn.";
      break;
    case game_phase::turn_o:
      os << "It is O's turn.";
      break;
    case game_phase::win_x:
      os << "X has won the game.";
      break;
    case game_phase::win_o:
      os << "O has won the game.";
      break;
    case game_phase::draw:
      os << "The game ended in a Cat's Game.";
      break;
    default:
      break;
    }
    return os;
  }

}

#endif
