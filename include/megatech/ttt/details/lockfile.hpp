#ifndef MEGATECH_TTT_DETAILS_LOCKFILE_HPP
#define MEGATECH_TTT_DETAILS_LOCKFILE_HPP

#include <cstdio>

#include <filesystem>

namespace megatech::ttt::details {

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

}

#endif
