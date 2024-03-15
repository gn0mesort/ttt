/**
 * @file lockfile.cpp
 * @brief File locking utility object.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#include "megatech/ttt/details/lockfile.hpp"

#include <string>
#include <stdexcept>

namespace megatech::ttt::details {

  lockfile::lockfile(const std::filesystem::path& path) : m_lock_path{ std::filesystem::absolute(path) } {
    // This doesn't check for existence or any other issues because it doesn't matter for the lockfile.
    // The only criteria for locking is that we aren't using a path that ends in a directory separator basically.
    if (!m_lock_path.has_filename())
    {
      throw std::runtime_error{ "The input path does not refer to a file." };
    }
    // This tingles my spidey-sense. I don't know that it's especially inefficient but it feels like it should be.
    auto name = std::string{ ".~lock" }.append(m_lock_path.filename());
    m_lock_path.replace_filename(name);
  }

  lockfile::~lockfile() noexcept {
    unlock();
  }

  bool lockfile::try_lock() {
    // If we're already locked then we fail.
    if (m_underlying)
    {
      return false;
    }
    // If we're not already locked then attempt to acquire the lock.
    m_underlying = std::fopen(m_lock_path.c_str(), "wx");
    // If the lock could not be acquired (for any reason) then we fail.
    if (!m_underlying)
    {
      return false;
    }
    // Ensure the file is flushed to the disk after we acquire it.
    // I don't know that this is necessary but I don't trust that the file will always be flushed to the disk
    // otherwise.
    if (std::fflush(m_underlying))
    {
      // If we failed to flush then unlock and return failure.
      unlock();
      return false;
    }
    return true;
  }

  void lockfile::lock() {
    if (!try_lock())
    {
      throw std::runtime_error{ "The desired lockfile was unavailable." };
    }
  }

  void lockfile::unlock() {
    // Only unlock if we are actually locked.
    if (m_underlying)
    {
      std::fclose(m_underlying);
      std::remove(m_lock_path.c_str());
      m_underlying = nullptr;
    }
  }

}
