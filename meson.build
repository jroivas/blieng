project('blieng', 'cpp', 'c')

qtcore = dependency('Qt5Core')

blieng_inc = include_directories('blieng')
toplevel_inc = include_directories('.')

gmock = subproject('gmock')

subdir('blieng')
if not meson.is_subproject()
    subdir('testtools')
    subdir('tests')
endif
