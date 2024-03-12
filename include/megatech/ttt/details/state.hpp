#ifndef MEGATECH_TTT_DETAILS_STATE_HPP
#define MEGATECH_TTT_DETAILS_STATE_HPP

#include <cstddef>
#include <cinttypes>

#include "../enums.hpp"

namespace megatech::ttt::details {

  class state final {
  private:
    static constexpr const std::uint32_t   GAME_MODE_MASK{ 0x80'00'00'00 };
    static constexpr const std::uint32_t  GAME_PHASE_MASK{ 0x70'00'00'00 };
    static constexpr const std::uint32_t GAME_UNUSED_MASK{ 0x0f'fc'00'00 };
    static constexpr const std::uint32_t  GAME_BOARD_MASK{ 0x00'03'ff'ff };

    static constexpr const std::uint32_t  BOARD_UPPER_ROW_MASK{ 0x00'00'00'3f };
    static constexpr const std::uint32_t BOARD_MIDDLE_ROW_MASK{ 0x00'00'0f'c0 };
    static constexpr const std::uint32_t BOARD_BOTTOM_ROW_MASK{ 0x00'03'f0'00 };
    static constexpr const std::uint32_t   BOARD_LEFT_COLUMN_MASK{ 0x00'00'30'c3 };
    static constexpr const std::uint32_t BOARD_CENTER_COLUMN_MASK{ 0x00'00'c3'0c };
    static constexpr const std::uint32_t  BOARD_RIGHT_COLUMN_MASK{ 0x00'03'0c'30 };
    static constexpr const std::uint32_t BOARD_LEFT_TO_RIGHT_DIAGONAL_MASK{ 0x00'03'03'03 };
    static constexpr const std::uint32_t BOARD_RIGHT_TO_LEFT_DIAGONAL_MASK{ 0x00'00'33'30 };
    static constexpr const std::size_t BOARD_MIDDLE_ROW_SHIFT{ 6 };
    static constexpr const std::size_t BOARD_BOTTOM_ROW_SHIFT{ 12 };

    static constexpr const std::uint32_t   BOARD_LEFT_CELL_MASK{ 0x03 };
    static constexpr const std::uint32_t BOARD_CENTER_CELL_MASK{ 0x0c };
    static constexpr const std::uint32_t  BOARD_RIGHT_CELL_MASK{ 0x30 };
    static constexpr const std::size_t BOARD_CENTER_CELL_SHIFT{ 2 };
    static constexpr const std::size_t  BOARD_RIGHT_CELL_SHIFT{ 4 };

    static constexpr const std::uint32_t BOARD_CELL_EMPTY{ 0x00 };
    static constexpr const std::uint32_t     BOARD_CELL_X{ 0x01 };
    static constexpr const std::uint32_t     BOARD_CELL_O{ 0x02 };

    static constexpr const std::uint32_t COUNT_X_MASK{ 0x00'01'55'55 };
    static constexpr const std::uint32_t COUNT_O_MASK{ 0x00'02'aa'aa };


    std::uint32_t m_data{ };
  public:
    static constexpr const std::uint32_t ALL_CELL_BITS{ 0x03 };

    state() = default;
    explicit state(const std::uint32_t data);
    state(const state& other) = default;
    state(state&& other) = default;

    ~state() noexcept = default;

    explicit operator std::uint32_t() const;
    state& operator=(const state& rhs) = default;
    state& operator=(state&& rhs) = default;

    game_mode mode() const;
    void mode(const game_mode gm);
    game_phase phase() const;
    void phase(const game_phase gp);
    std::uint32_t board() const;
    bool is_board_empty() const;
    bool is_board_full() const;
    bool is_row_x(const std::size_t row) const;
    bool is_row_o(const std::size_t row) const;
    bool is_column_x(const std::size_t column) const;
    bool is_column_o(const std::size_t column) const;
    bool is_left_diagonal_x() const;
    bool is_left_diagonal_o() const;
    bool is_right_diagonal_x() const;
    bool is_right_diagonal_o() const;
    bool is_cell_empty(const std::size_t column, const std::size_t row) const;
    bool is_cell_x(const std::size_t column, const std::size_t row) const;

    std::size_t filled_cells() const;
    std::size_t count_x() const;
    std::size_t count_o() const;

    cell_contents cell(const std::size_t column, const std::size_t row) const;
    void cell(const std::size_t column, const std::size_t row, const cell_contents value);

  };

}

#endif
