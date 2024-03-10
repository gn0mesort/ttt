#include "megatech/ttt/details/interpreter.hpp"

#include <cstring>

namespace megatech::ttt::details {

  interpreter::interpreter() : interpreter{ 65536 } { }

  interpreter::interpreter(const std::size_t cells) : m_ram(cells) { }

  void interpreter::initialize(const char *const program, const std::size_t program_length, const char* const input,
                               const std::size_t input_length) {
    m_rom.resize(program_length + input_length + MAX_STACK_SIZE * ADDRESS_SIZE);
    m_input.base = 0;
    m_input.length = input_length;
    m_input.pointer = 0;
    std::memcpy(&m_rom[m_input.base], input, input_length);
    m_instruction.base = m_input.base + m_input.length;
    m_instruction.length = program_length;
    m_instruction.pointer = 0;
    std::memcpy(&m_rom[m_instruction.base], program, program_length);
    m_stack.base = m_instruction.base + m_instruction.length;
    m_stack.length = MAX_STACK_SIZE * ADDRESS_SIZE;
    m_stack.pointer = 0;
    std::memset(&m_rom[m_stack.base], 0, m_stack.length);
    m_data.base = 0;
    m_data.length = m_ram.size();
    m_data.pointer = 0;
    std::memset(m_ram.data(), 0, m_ram.size());
  }

  bool interpreter::step(std::vector<char>& output) {
    switch (m_rom[m_instruction.base + m_instruction.pointer])
    {
    case '>':
      ++m_data.pointer;
      if (m_data.pointer >= m_data.length)
      {
        m_data.pointer = 0;
      }
      break;
    case '<':
      --m_data.pointer;
      if (m_data.pointer >= m_data.length)
      {
        m_data.pointer = m_data.length - 1;
      }
      break;
    case '+':
      m_ram[m_data.base + m_data.pointer] =
        static_cast<char>(static_cast<unsigned char>(m_ram[m_data.base + m_data.pointer]) + 1);
      break;
    case '-':
      m_ram[m_data.base + m_data.pointer] =
        static_cast<char>(static_cast<unsigned char>(m_ram[m_data.base + m_data.pointer]) - 1);
      break;
    case '.':
      output.push_back(m_ram[m_data.base + m_data.pointer]);
      break;
    case ',':
      if (m_input.pointer >= m_input.length)
      {
        m_ram[m_data.base + m_data.pointer] = '\0';
      }
      else
      {
        m_ram[m_data.base + m_data.pointer] = m_rom[m_input.base + m_input.pointer];
        ++m_input.pointer;
      }
      break;
    case '[':
      if (!m_ram[m_data.base + m_data.pointer])
      {
        while (m_instruction.pointer < m_instruction.length &&
               m_rom[m_instruction.base + m_instruction.pointer] != ']')
        {
          ++m_instruction.pointer;
        }
      }
      else
      {
        const auto address = m_instruction.pointer - 1;
        std::memcpy(&m_rom[m_stack.base + m_stack.pointer], &address, ADDRESS_SIZE);
        m_stack.pointer += ADDRESS_SIZE;
      }
      break;
    case ']':
      if (m_stack.pointer != m_stack.base)
      {
        if (m_ram[m_data.base + m_data.pointer])
        {
          m_instruction.pointer = m_rom[m_stack.base + m_stack.pointer - ADDRESS_SIZE];
        }
        m_stack.pointer -= ADDRESS_SIZE;
      }
      break;
    default:
      break;
    }
    ++m_instruction.pointer;
    return m_instruction.pointer < m_instruction.length;
  }

  std::vector<char> interpreter::execute(const char *const program, const std::size_t program_length,
                                         const char *const input, const std::size_t input_length) {
    initialize(program, program_length, input, input_length);
    auto output = std::vector<char>{ };
    while (step(output));
    return output;
  }

}
