project('blieng', 'cpp', 'c')

add_global_arguments('-std=c11', language : 'c')
add_global_arguments('-std=c++11', language : 'cpp')
add_global_arguments('-fPIC', language : 'cpp')

qtcore = dependency('Qt5Core')

blieng_inc = include_directories('blieng')

gmock = subproject('gmock')

subdir('blieng')
subdir('tests')
