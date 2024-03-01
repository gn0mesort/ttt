#ifndef MEGATECH_TTT_FILE_LOCK_HPP
#define MEGATECH_TTT_FILE_LOCK_HPP

#include <cstdio>

#include <filesystem>

namespace megatech::ttt {

  class lockfile final {
  private:
    std::filesystem::path m_lock_path{ };
    FILE* m_underlying{ };
  public:
    lockfile(const std::filesystem::path& path);
    lockfile(const lockfile& other) = delete;
    lockfile(lockfile&& other) = default;

    ~lockfile() noexcept;

    lockfile& operator=(const lockfile& rhs) = delete;
    lockfile& operator=(lockfile&& rhs) = default;

    bool try_lock();
    void lock();
    void unlock();
  };

  class scoped_file_lock final {
  private:
    lockfile* m_lock{ };
  public:
    scoped_file_lock(lockfile& lock);
    scoped_file_lock(const scoped_file_lock& other) = delete;
    scoped_file_lock(scoped_file_lock&& other) = default;

    ~scoped_file_lock() noexcept;

    scoped_file_lock& operator=(const scoped_file_lock& rhs) = delete;
    scoped_file_lock& operator=(scoped_file_lock&& rhs) = default;
  };

}

#endif
