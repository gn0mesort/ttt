#include "megatech/ttt/game.hpp"

#include <cstring>

#include <fstream>

#include "megatech/ttt/details/data_file.hpp"

#define BSWAP32(x) \
  ((((x) & 0x00'00'00'ff) << 24) | \
    (((x) & 0x00'00'ff'00) << 8) | \
    (((x) & 0x00'ff'00'00) >> 8) | \
    (((x) & 0xff'00'00'00) >> 24))

namespace megatech::ttt {

  game::game(const std::filesystem::path& path) : m_path{ std::filesystem::absolute(path) },
                                                  m_lock{ m_path } {
    m_lock.lock();
    auto stat = std::filesystem::status(m_path);
    // Can we even stat the file?
    if (!std::filesystem::status_known(stat))
    {
      throw std::runtime_error{ "The status of the game data file could not be determined." };
    }
    // If the game data file exists then check if it is regular.
    if (std::filesystem::exists(stat))
    {
      // If the game data file is regular then read it back.
      if (std::filesystem::is_regular_file(stat))
      {
        auto f_in = std::ifstream{ m_path, std::ios::binary | std::ios::ate };
        if (f_in.tellg() < 0 || static_cast<std::size_t>(f_in.tellg()) < sizeof(details::data_file_header))
        {
          throw std::runtime_error{ "The requested game data file is too short to be valid." };
        }
        f_in.seekg(0, std::ios::beg);
        auto header = details::data_file_header{ };
        f_in.read(reinterpret_cast<char*>(&header), sizeof(details::data_file_header));
        if (std::memcmp(header.magic, details::DATA_FILE_HEADER_MAGIC, details::DATA_FILE_HEADER_MAGIC_LENGTH) != 0 ||
            header.version != details::DATA_FILE_VERSION_1)
        {
          throw std::runtime_error{ "The game data file is corrupt." };
        }
        auto body = details::data_file_body_v1{ };
        f_in.read(reinterpret_cast<char*>(&body), sizeof(details::data_file_body_v1));
        switch (body.endianness)
        {
        case details::REVERSE_ENDIANNESS:
          m_state = BSWAP32(body.state);
          break;
        case details::CORRECT_ENDIANNESS:
          m_state = body.state;
          break;
        default:
          throw std::runtime_error{ "The game data file is corrupt or it was written with an unknown byte order." };
        }
      }
      else
      {
        throw std::runtime_error{ "The requested game data file is not a regular file." };
      }
    }
    // If the game data file does not exist start an empty game.
    else
    {
      m_state = 0;
    }
  }

  game::~game() noexcept {
    {
      auto f_out = std::ofstream{ m_path, std::ios::binary | std::ios::trunc };
      auto header = details::data_file_header{ };
      std::memcpy(header.magic, details::DATA_FILE_HEADER_MAGIC, details::DATA_FILE_HEADER_MAGIC_LENGTH);
      header.version = details::DATA_FILE_VERSION_1;
      f_out.write(reinterpret_cast<char*>(&header), sizeof(details::data_file_header));
      auto body = details::data_file_body_v1{ };
      body.endianness = details::CORRECT_ENDIANNESS;
      body.state = m_state;
      f_out.write(reinterpret_cast<char*>(&body), sizeof(details::data_file_body_v1));
    }
    m_lock.unlock();
  }

}
