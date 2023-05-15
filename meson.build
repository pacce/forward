project('forward', 'cpp', version: '0.1.0', default_options : ['warning_level=3', 'cpp_std=c++20'])

compiler      = meson.get_compiler('cpp')
sources       = []
include       = include_directories('include')
dependencies  = [
  dependency('dimension')
  , dependency('geometry')
  , dependency('mesh')
  ]

forward_dep = declare_dependency(
  include_directories   : include
  , dependencies        : dependencies
  )

if not meson.is_subproject()
  subdir('example')
endif
