# @file meson.build
# @brief Test build script.
# @author Alexander Rothman <gnomesort@megate.ch>
# @date 2024
# @copyright AGPL-3.0+
if get_option('buildtype') == 'debug' or get_option('buildtype') == 'debugoptimized'
  file_locking_test_exe = executable('file_locking_test', files('file_locking.cpp'), dependencies: ttt_dep)
  test('File Locking', file_locking_test_exe, is_parallel: false)
  game_file_io_test_exe = executable('game_file_io_test', files('game_file_io.cpp'), dependencies: ttt_dep)
  test('Game File I/O', game_file_io_test_exe, is_parallel: false)
  if true
    home_directories_exe = executable('home_directories_test', files('home_directories.cpp'), dependencies: ttt_dep)
    dir = meson.current_build_dir()
    if host_machine.system() == 'windows' or host_machine.system() == 'cygwin'
      environ_1 = environment()
      environ_1.set('HOMEPATH', dir)
      environ_1.set('RESULT_HOME', dir)
      test('Windows Home Directory Location (HOMEPATH)', home_directories_exe, env: environ_1, is_parallel: false)
      environ_2 = environment()
      environ_2.set('USERPROFILE', dir)
      environ_2.set('RESULT_HOME', dir)
      test('Windows Home Directory Location (USERPROFILE)', home_directories_exe, env: environ_2, is_parallel: false)
    else
      environ = environment()
      environ.set('HOME', dir)
      environ.set('RESULT_HOME', dir)
      test('POSIX Home Directory Location', home_directories_exe, env: environ, is_parallel: false)
    endif
    if true
      environ = environment()
      environ.set('MEGATECH_TTT_HOME', dir)
      environ.set('RESULT_HOME', dir)
      test('Generic Home Directory Location', home_directories_exe, env: environ, is_parallel: false)
    endif
  endif
  turn_taking_test_exe = executable('turn_taking_test', files('turn_taking.cpp'), dependencies: ttt_dep)
  test('Turn Taking', turn_taking_test_exe, is_parallel: false)
  enum_strings_test_exe = executable('enum_strings_test', files('enum_strings.cpp'),
                                           dependencies: ttt_dep)
  test('Enum Type Strings', enum_strings_test_exe, is_parallel: false)
  frobnication_test_exe = executable('frobnication_test', files('frobnication.cpp'), dependencies: ttt_dep)
  test('Frobnication', frobnication_test_exe)
  interpreter_test_exe = executable('interpreter_test', files('interpreter.cpp'), dependencies: ttt_dep)
  test('Interpreter', interpreter_test_exe)
  utilities_test_exe = executable('utilities_test', files('utilities.cpp'), dependencies: ttt_dep)
  test('General Utilities', utilities_test_exe)
  strategy_test_exe = executable('strategy_test', files('strategy.cpp'), dependencies: ttt_dep)
  test('Strategy', strategy_test_exe)
endif
