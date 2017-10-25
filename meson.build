project('blieng', 'cpp', 'c')

blieng_inc = include_directories('blieng')
toplevel_inc = include_directories('.')

gmock = subproject('gmock')
box2d = subproject('box2d')

subdir('blieng')
subdir('examples')
if not meson.is_subproject() and not meson.is_cross_build()
    subdir('testtools')
    subdir('tests')
endif
