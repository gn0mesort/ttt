project(
  'ttt',
  [ 'cpp' ],
  version: '1.0.0',
  license: 'AGPL-3.0+',
  default_options: [
    'cpp_std=c++20',
    'warning_level=3',
    'b_ndebug=if-release',
    'b_pie=true',
    'b_sanitize=address',
    'cpp_args=-fstrict-aliasing -Wstrict-aliasing=3'
  ],
  subproject_dir: 'lib'
)

ttt_config = configuration_data()
if true
  os = host_machine.system()
  ttt_config.set_quoted('CONFIGURATION_OPERATING_SYSTEM_NAME', os)
  if os == 'windows' or os == 'cygwin'
    ttt_config.set('CONFIGURATION_OPERATING_SYSTEM_WINDOWS', 1)
  else
    ttt_config.set('CONFIGURATION_OPERATING_SYSTEM_POSIX', 1)
  endif
endif

ttt_lib_incs = [
  include_directories('include')
]

ttt_lib_srcs = [
  configure_file(input: files('generated/configuration.hpp.in'), output: 'configuration.hpp',
                 configuration: ttt_config),
  files('src/megatech/ttt/game.cpp', 'src/megatech/ttt/utility.cpp', 'src/megatech/ttt/enums.cpp'),
  files('src/megatech/ttt/details/lockfile.cpp', 'src/megatech/ttt/details/state.cpp')
]

ttt_lib = library(meson.project_name(), ttt_lib_srcs, include_directories: ttt_lib_incs)
ttt_dep = declare_dependency(link_with: ttt_lib, include_directories: ttt_lib_incs)

ttt_new_game_srcs = [
  files('src/new_game.cpp')
]

ttt_new_game_exe = executable('@0@-new-game'.format(meson.project_name()), ttt_new_game_srcs, dependencies: ttt_dep)

ttt_delete_game_srcs = [
  files('src/delete_game.cpp')
]

ttt_delete_game_exe = executable('@0@-delete-game'.format(meson.project_name()), ttt_delete_game_srcs,
                                 dependencies: ttt_dep)

ttt_display_game_srcs = [
  files('src/display_game.cpp')
]

ttt_display_game_exe = executable('@0@-display-game'.format(meson.project_name()), ttt_display_game_srcs,
                                  dependencies: ttt_dep)

ttt_take_turn_srcs = [
  files('src/take_turn.cpp')
]

ttt_take_turn_exe = executable('@0@-take-turn'.format(meson.project_name()), ttt_take_turn_srcs, dependencies: ttt_dep)

subdir('tests')
