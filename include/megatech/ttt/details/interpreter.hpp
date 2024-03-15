/**
 * @file interpreter.hpp
 * @brief Embedded language interpreter.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_DETAILS_INTERPRETER_HPP
#define MEGATECH_TTT_DETAILS_INTERPRETER_HPP

#include <cstddef>

#include <span>
#include <vector>
#include <ranges>

namespace megatech::ttt::details {

  /**
   * @brief An object representing an embedded programming language interpreter.
   */
  class interpreter final {
  private:
    static constexpr std::size_t MAX_STACK_SIZE{ 1024 };
    static constexpr std::size_t ADDRESS_SIZE{ sizeof(std::size_t) };

    struct offset final {
      std::size_t base;
      std::size_t length;
      std::size_t pointer;
    };

    offset m_input{ };
    offset m_instruction{ };
    offset m_stack{ };
    offset m_data{ };

    std::vector<char> m_rom{ };
    std::vector<char> m_ram{ };

    void initialize(const char *const program, const std::size_t program_length, const char* const input,
                    const std::size_t input_length);
    bool step(std::vector<char>& outpu);
    std::vector<char> execute(const char *const program, const std::size_t program_length, const char *const input,
                              const std::size_t input_length);
  public:
    /**
     * @brief Create a default initialized interpreter.
     * @details This creates an interpreter with 64KiB of RAM.
     */
    interpreter();

    /**
     * @brief Create an interpreter with a specific amount of RAM.
     * @param cells The number of 1 character cells in the interpreter's RAM.
     */
    explicit interpreter(const std::size_t cells);

    /**
     * @brief Create an interpreter as a copy of another.
     * @param other The interpreter to copy.
     */
    interpreter(const interpreter& other) = default;

    /**
     * @brief Create an interpreter by moving another.
     * @param other The interpreter to move.
     */
    interpreter(interpreter&& other) = default;

    /**
     * @brief Destroy an interpreter.
     */
    ~interpreter() noexcept = default;

    /**
     * @brief Assign an interpreter to a copy of another.
     * @param rhs The interpreter to copy.
     * @return A reference to the assigned object.
     */
    interpreter& operator=(const interpreter& rhs) = default;

    /**
     * @brief Assign an interpreter by moving another.
     * @param rhs The interpreter to move.
     * @return A reference to the assigned object.
     */
    interpreter& operator=(interpreter&& rhs) = default;

    /**
     * @brief Execute an interpreter program.
     * @details The program range will be reinterpreted as a range of characters.
     * @tparam ProgramRange The type of the range containing the program.
     * @param program The program to run as a contiguous range.
     * @return The program's output as a vector of characters.
     */
    template <std::ranges::contiguous_range ProgramRange>
    std::vector<char> execute(ProgramRange&& program);


    /**
     * @brief Execute an interpreter program with the given input.
     * @details The program range and input range will be reinterpreted as ranges of characters.
     * @tparam ProgramRange The type of the range containing the program.
     * @tparam InputRange The type of the range containing the input.
     * @param program The program to run as a contiguous range.
     * @param input The input to feed to the program as a contiguous range.
     * @return The program's output as a vector of characters.
     */
    template <std::ranges::contiguous_range ProgramRange, std::ranges::contiguous_range InputRange>
    std::vector<char> execute(ProgramRange&& program, InputRange&& input);
  };

  template <std::ranges::contiguous_range ProgramRange>
  std::vector<char> interpreter::execute(ProgramRange&& program) {
    constexpr auto SIZE_BYTES = sizeof(std::ranges::range_value_t<ProgramRange>);
    return execute(reinterpret_cast<const char*>(std::ranges::cdata(program)), std::ranges::size(program) * SIZE_BYTES,
                   nullptr, 0);
  }

  template <std::ranges::contiguous_range ProgramRange, std::ranges::contiguous_range InputRange>
  std::vector<char> interpreter::execute(ProgramRange&& program, InputRange&& input) {
    constexpr auto PROGRAM_SIZE_BYTES = sizeof(std::ranges::range_value_t<ProgramRange>);
    constexpr auto INPUT_SIZE_BYTES = sizeof(std::ranges::range_value_t<InputRange>);
    return execute(reinterpret_cast<const char*>(std::ranges::cdata(program)),
                   std::ranges::size(program) * PROGRAM_SIZE_BYTES,
                   reinterpret_cast<const char*>(std::ranges::cdata(input)),
                   std::ranges::size(input) * INPUT_SIZE_BYTES);
  }

}

#endif
