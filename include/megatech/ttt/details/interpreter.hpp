#ifndef MEGATECH_TTT_DETAILS_INTERPRETER_HPP
#define MEGATECH_TTT_DETAILS_INTERPRETER_HPP

#include <cstddef>

#include <span>
#include <vector>
#include <ranges>

namespace megatech::ttt::details {

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
    interpreter();
    explicit interpreter(const std::size_t cells);
    interpreter(const interpreter& other) = default;
    interpreter(interpreter&& other) = default;

    ~interpreter() noexcept = default;

    interpreter& operator=(const interpreter& rhs) = default;
    interpreter& operator=(interpreter&& rhs) = default;

    template <std::ranges::contiguous_range ProgramRange>
    std::vector<char> execute(ProgramRange&& program);


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
