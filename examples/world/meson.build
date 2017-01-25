boost = dependency('boost',
    modules: [
        'system',
        'filesystem',
        'serialization',
        'random',
        'date_time'
    ])

world_example = executable('world_example',
    sources: [
        'world_example.cpp'
    ],
    include_directories: [
        blieng_inc,
        toplevel_inc
    ],
    link_with: [
        blieng_lib
    ],
    link_args: [
        '-lpthread'
    ],
    cpp_args : ['-fPIC', '-std=c++11'],
    dependencies: [boost]
)
