boost = dependency('boost',
    modules: [
        'system',
        'filesystem',
        'serialization',
        'random',
        'date_time'
    ])

data_example = executable('data_example',
    sources: [
        'data_example.cpp'
    ],
    include_directories: [
        blieng_inc,
        toplevel_inc
    ],
    link_with: [
        blieng_lib
    ],
    cpp_args : ['-fPIC', '-std=c++11'],
    dependencies: [boost]
)
