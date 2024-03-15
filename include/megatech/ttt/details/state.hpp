/**
 * @file state.hpp
 * @brief Game state management object.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_DETAILS_STATE_HPP
#define MEGATECH_TTT_DETAILS_STATE_HPP

#include <cstddef>
#include <cinttypes>

#include "../enums.hpp"

namespace megatech::ttt::details {

  class state final {
  private:
    static constexpr std::uint32_t   GAME_MODE_MASK{ 0x80'00'00'00 };
    static constexpr std::uint32_t  GAME_PHASE_MASK{ 0x70'00'00'00 };
    static constexpr std::uint32_t GAME_UNUSED_MASK{ 0x0f'fc'00'00 };
    static constexpr std::uint32_t  GAME_BOARD_MASK{ 0x00'03'ff'ff };

    static constexpr std::uint32_t  BOARD_UPPER_ROW_MASK{ 0x00'00'00'3f };
    static constexpr std::uint32_t BOARD_MIDDLE_ROW_MASK{ 0x00'00'0f'c0 };
    static constexpr std::uint32_t BOARD_BOTTOM_ROW_MASK{ 0x00'03'f0'00 };
    static constexpr std::uint32_t   BOARD_LEFT_COLUMN_MASK{ 0x00'00'30'c3 };
    static constexpr std::uint32_t BOARD_CENTER_COLUMN_MASK{ 0x00'00'c3'0c };
    static constexpr std::uint32_t  BOARD_RIGHT_COLUMN_MASK{ 0x00'03'0c'30 };
    static constexpr std::uint32_t BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK{ 0x00'03'03'03 };
    static constexpr std::uint32_t BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK{ 0x00'00'33'30 };
    static constexpr std::size_t BOARD_MIDDLE_ROW_SHIFT{ 6 };
    static constexpr std::size_t BOARD_BOTTOM_ROW_SHIFT{ 12 };

    static constexpr std::uint32_t   BOARD_LEFT_CELL_MASK{ 0x03 };
    static constexpr std::uint32_t BOARD_CENTER_CELL_MASK{ 0x0c };
    static constexpr std::uint32_t  BOARD_RIGHT_CELL_MASK{ 0x30 };
    static constexpr std::size_t BOARD_CENTER_CELL_SHIFT{ 2 };
    static constexpr std::size_t  BOARD_RIGHT_CELL_SHIFT{ 4 };

    static constexpr std::uint32_t BOARD_CELL_EMPTY{ 0x00 };
    static constexpr std::uint32_t     BOARD_CELL_X{ 0x01 };
    static constexpr std::uint32_t     BOARD_CELL_O{ 0x02 };

    static constexpr std::uint32_t COUNT_X_MASK{ 0x00'01'55'55 };
    static constexpr std::uint32_t COUNT_O_MASK{ 0x00'02'aa'aa };

    std::uint32_t m_data{ };
  public:
    /**
     * @brief A constant representing all possible game board cell bits.
     */
    static constexpr std::uint32_t ALL_CELL_BITS{ 0x03 };

    /**
     * @brief Create a default initialized state.
     */
    state() = default;

    /**
     * @brief Create a state with the specified data.
     * @throw std::runtime_error If the data does not represent a valid game state.
     */
    explicit state(const std::uint32_t data);

    /**
     * @brief Create a state as a copy of another.
     * @param other The state to copy.
     */
    state(const state& other) = default;

    /**
     * @brief Create a state by moving another.
     * @param other The state to move.
     */
    state(state&& other) = default;

    /**
     * @brief Destroy a state.
     */
    ~state() noexcept = default;

    /**
     * @brief Convert the state into a 32-bit unsigned integer.
     */
    explicit operator std::uint32_t() const;

    /**
     * @brief Assign a state as a copy of another.
     * @param rhs The state to copy.
     * @return A reference to the assigned object.
     */
    state& operator=(const state& rhs) = default;

    /**
     * @brief Assign a state by moving another.
     * @param rhs The state to move.
     * @return A reference to the assigned object.
     */
    state& operator=(state&& rhs) = default;

    /**
     * @brief Retrieve the current game_mode.
     * @return The current game_mode.
     */
    game_mode mode() const;

    /**
     * @brief Set the current game_mode.
     * @param gm The new game_mode.
     * @throw std::runtime_error If the game_mode is ill formed.
     */
    void mode(const game_mode gm);

    /**
     * @brief Retrieve the current game_phase.
     * @return The current game_phase.
     */
    game_phase phase() const;

    /**
     * @brief Set the current game_phase.
     * @param gp The new game_phase.
     * @throw std::runtime_error If the game_phase is ill formed.
     */
    void phase(const game_phase gp);

    /**
     * @brief Retrieve the current game board.
     * @return A 32-bit unsigned integer value representing the current game board.
     */
    std::uint32_t board() const;

    /**
     * @brief Check if the game board is empty.
     * @return True if every cell is unmarked. False in all other cases.
     */
    bool is_board_empty() const;

    /**
     * @brief Check if the game board is full.
     * @return True if there are 9 marks on the game board. False in all other cases.
     */
    bool is_board_full() const;

    /**
     * @brief Determine if the given row is marked with all X's.
     * @param row The row to query.
     * @return True if the given row is marked with all X's. False in all other cases.
     * @throw std::runtime_error If the row index is out of range.
     */
    bool is_row_x(const std::size_t row) const;

    /**
     * @brief Determine if the given row is marked with all O's.
     * @param row The row to query.
     * @return True if the given row is marked with all O's. False in all other cases.
     * @throw std::runtime_error If the row index is out of range.
     */
    bool is_row_o(const std::size_t row) const;

    /**
     * @brief Determine if the given column is marked with all X's.
     * @param column The column to query.
     * @return True if the given column is marked with all X's. False in all other cases.
     * @throw std::runtime_error If the column index is out of range.
     */
    bool is_column_x(const std::size_t column) const;

    /**
     * @brief Determine if the given column is marked with all O's.
     * @param column The column to query.
     * @return True if the given column is marked with all O's. False in all other cases.
     * @throw std::runtime_error If the column index is out of range.
     */
    bool is_column_o(const std::size_t column) const;

    /**
     * @brief Determine if the left-to-right game board diagonal is marked with all X's
     * @return True if the left-to-right diagonal is marked with all X's. False in all other cases.
     */
    bool is_left_diagonal_x() const;

    /**
     * @brief Determine if the left-to-right game board diagonal is marked with all O's
     * @return True if the left-to-right diagonal is marked with all O's. False in all other cases.
     */
    bool is_left_diagonal_o() const;

    /**
     * @brief Determine if the right-to-left game board diagonal is marked with all X's
     * @return True if the right-to-left diagonal is marked with all X's. False in all other cases.
     */
    bool is_right_diagonal_x() const;

    /**
     * @brief Determine if the right-to-left game board diagonal is marked with all O's
     * @return True if the right-to-left diagonal is marked with all O's. False in all other cases.
     */
    bool is_right_diagonal_o() const;

    /**
     * @brief Determine if the given cell is unmarked.
     * @param column The column index of the cell.
     * @param row The row index of the cell.
     * @return True if the cell at (column, row) is unmarked. False in all other cases.
     * @throw std::runtime_error If the column or row index is out of range.
     */
    bool is_cell_empty(const std::size_t column, const std::size_t row) const;

    /**
     * @brief Determine if the given cell is marked with an X.
     * @param column The column index of the cell.
     * @param row The row index of the cell.
     * @return True if the cell at (column, row) is marked with an X. False in all other cases.
     * @throw std::runtime_error If the column or row index is out of range.
     */
    bool is_cell_x(const std::size_t column, const std::size_t row) const;

    /**
     * @brief Determine how many cells on the game board are marked.
     * @return The count of marked cells.
     */
    std::size_t filled_cells() const;

    /**
     * @brief Determine how many cells on the game board are marked with an X.
     * @return The count of marked cells.
     */
    std::size_t count_x() const;

    /**
     * @brief Determine how many cells on the game board are marked with an O.
     * @return The count of marked cells.
     */
    std::size_t count_o() const;

    /**
     * @brief Query the content of the cell at the given position.
     * @param column The column index of the cell.
     * @param row The row index of the cell.
     * @return The content of the desired cell.
     * @throw std::runtime_error If the column or row index is out of range.
     */
    cell_contents cell(const std::size_t column, const std::size_t row) const;

    /**
     * @brief Set the content of the cell at the given position.
     * @param column The column index of the cell.
     * @param row The row index of the cell.
     * @param value The value to assign to the cell.
     * @throw std::runtime_error If the column or row index is out of range.
     */
    void cell(const std::size_t column, const std::size_t row, const cell_contents value);

  };

}

#endif
