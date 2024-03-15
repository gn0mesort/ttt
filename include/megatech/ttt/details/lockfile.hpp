/**
 * @file lockfile.hpp
 * @brief File locking utility object.
 * @author Alexander Rothman <gnomesort@megate.ch>
 * @date 2024
 * @copyright AGPL-3.0+
 */
#ifndef MEGATECH_TTT_DETAILS_LOCKFILE_HPP
#define MEGATECH_TTT_DETAILS_LOCKFILE_HPP

#include <cstdio>

#include <filesystem>

namespace megatech::ttt::details {

  /**
   * An object representing a lockfile.
   */
  class lockfile final {
  private:
    std::filesystem::path m_lock_path{ };
    FILE* m_underlying{ };
  public:
    /**
     * @brief Create a lockfile that locks the input file path.
     * @param path The path to the file to lock.
     * @throw std::runtime_error If the input path does not refer to a file.
     */
    explicit lockfile(const std::filesystem::path& path);

    /// @cond
    lockfile(const lockfile& other) = delete;
    /// @endcond

    /**
     * @brief Create a lockfile by moving another.
     * @param other The lockfile to move.
     */
    lockfile(lockfile&& other) = default;

    /**
     * @brief Destroy a lockfile.
     * @details If the file is locked, it will be unlocked during destruction.
     */
    ~lockfile() noexcept;

    /// @cond
    lockfile& operator=(const lockfile& rhs) = delete;
    /// @endcond

    /**
     * @brief Assign a lockfile by moving another.
     * @param rhs The lockfile to move.
     * @return A reference to the assigned lockfile.
     */
    lockfile& operator=(lockfile&& rhs) = default;

    /**
     * @brief Attempt to lock the lockfile.
     * @return True if the lockfile locked the file successfully. False if in any other case.
     */
    bool try_lock();

    /**
     * @brief Lock the lockfile.
     * @throw std::runtime_error If locking failed for any reason.
     */
    void lock();

    /**
     * @brief Unlock the lockfile.
     */
    void unlock();
  };

}

#endif
