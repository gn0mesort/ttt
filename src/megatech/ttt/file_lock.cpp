#include "megatech/ttt/file_lock.hpp"

#include <string>
#include <stdexcept>

namespace megatech::ttt {

  lockfile::lockfile(const std::filesystem::path& path) : m_lock_path{ std::filesystem::absolute(path) } {
    if (!m_lock_path.has_filename())
    {
      throw std::runtime_error{ "The input path does not refer to a file." };
    }
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
    // If the lock could not be acquired (for any reason)
    if (!m_underlying)
    {
      return false;
    }
    // Ensure the file is flushed to the disk after we acquire it.
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
    if (m_underlying)
    {
      std::fclose(m_underlying);
      std::remove(m_lock_path.c_str());
    }
    m_underlying = nullptr;
  }

  scoped_file_lock::scoped_file_lock(lockfile& lock) : m_lock{ &lock } {
    m_lock->lock();
  }

  scoped_file_lock::~scoped_file_lock() noexcept {
    m_lock->unlock();
  }

}
