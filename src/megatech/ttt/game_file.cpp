#include "megatech/ttt/game_file.hpp"

#include <cstring>

#include <stdexcept>
#include <fstream>

#include "megatech/ttt/file_lock.hpp"

#define BSWAP32(x) \
  ((((x) & 0x00'00'00'ff) << 24) | \
    (((x) & 0x00'00'ff'00) << 8) | \
    (((x) & 0x00'ff'00'00) >> 8) | \
    (((x) & 0xff'00'00'00) >> 24))

namespace megatech::ttt {

  game_file::game_file(const std::filesystem::path& path) : m_path{ std::filesystem::absolute(path) } {
    auto stat = std::filesystem::status(m_path);
    // Can we even stat the file?
    if (!std::filesystem::status_known(stat))
    {
      throw std::runtime_error{ "Couldn't determine the status of the game data file." };
    }
    // If the file exists, is it a regular old file?
    if (std::filesystem::exists(stat) && std::filesystem::is_regular_file(stat))
    {
      throw std::runtime_error{ "The requested game data file was not a regular file." };
    }
    m_lock = new lockfile{ m_path };
    m_lock_guard = new scoped_file_lock{ *m_lock };
    if (std::filesystem::exists(stat))
    {
      auto f_in = std::ifstream{ m_path, std::ios::binary };
      f_in.read(reinterpret_cast<char*>(&m_file_header), sizeof(game_file::header));
      if (std::memcmp(m_file_header.magic, HEADER_MAGIC, HEADER_MAGIC_LENGTH) != 0 || m_file_header.version != 1)
      {
        throw std::runtime_error{ "The game data file is corrupt." };
      }
      f_in.read(reinterpret_cast<char*>(&m_file_body), sizeof(game_file::body_v1));
      switch (m_file_body.endianness)
      {
      case REVERSE_ENDIANNESS:
        m_file_body.state = BSWAP32(m_file_body.state);
        m_file_body.endianness = CORRECT_ENDIANNESS;
      case CORRECT_ENDIANNESS:
        break;
      default:
        throw std::runtime_error{ "The game data file is corrupt or it was written with an unknown byte order." };
      }
    }
    else
    {
      std::memcpy(&m_file_header.magic, HEADER_MAGIC, HEADER_MAGIC_LENGTH);
      m_file_header.version = 1;
      m_file_body.endianness = CORRECT_ENDIANNESS;
      m_file_body.state = 0;
    }
  }

  game_file::~game_file() noexcept {
    {
      auto f_out = std::ofstream{ m_path, std::ios::binary | std::ios::trunc };
      f_out.write(reinterpret_cast<char*>(&m_file_header), sizeof(game_file::header));
      f_out.write(reinterpret_cast<char*>(&m_file_body), sizeof(game_file::body_v1));
    }
    delete m_lock_guard;
    delete m_lock;
  }

}
