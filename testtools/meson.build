boost = dependency('boost',
    modules: [
        'system',
        'filesystem',
        'serialization',
        'random',
        'date_time'
    ]
    )

blieng_json = executable('blieng_json',
    'test_json.cpp',
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
    cpp_args : ['-fPIC', '-std=c++11'],
    link_args: [
        '-pthread', '-ldl'
    ],
    dependencies: [boost]
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
    cpp_args : ['-fPIC', '-std=c++11'],
    link_args: [
        '-pthread', '-ldl'
    ],
    dependencies: [boost]
    )

# Disabled tests for now since these need parameters
#test('test data', blieng_data)
#test('test json', blieng_json)
