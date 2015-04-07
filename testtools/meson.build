boost = dependency('boost',
    modules: [
        'system',
        'filesystem',
        'serialization',
        'random',
        'date_time'
    ]
    )

blieng_data = executable('blieng_data',
    'test_data.cpp',
    include_directories: [
        blieng_inc,
        toplevel_inc,
        gmock.get_variable('gmock_inc'),
        gmock.get_variable('gtest_inc')
    ],
    link_with: [
        gmock.get_variable('gmock_lib'),
        gmock.get_variable('gtest_lib'),
        blieng_lib
    ],
    link_args: [
        '-pthread', '-ldl'
    ],
    dependencies: [boost]
    )

test('test data', blieng_data)
