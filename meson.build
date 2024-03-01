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

ttt_lib_incs = [
  include_directories('include')
]

ttt_lib_srcs = [
  files('src/megatech/ttt/file_lock.cpp')
]

ttt_lib = library(meson.project_name(), ttt_lib_srcs, include_directories: ttt_lib_incs)
ttt_dep = declare_dependency(link_with: ttt_lib, include_directories: ttt_lib_incs)

subdir('tests')
