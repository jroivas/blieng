cppunit = dependency('cppunit')

boost = dependency('boost',
    modules: [
        'system',
        'filesystem',
        'serialization',
        'random',
        'date_time'
    ]
    )

bliengtest = executable('blieng_test',
    'auto_map_test.cpp',
    'auto_vector_test.cpp',
    'bliany_test.cpp',
    'bliobject_test.cpp',
    'card_test.cpp',
    'character_test.cpp',
    'configure_test.cpp',
    'datafile_test.cpp',
    'data_test.cpp',
    'eventlog_test.cpp',
    'item_test.cpp',
    'logging_test.cpp',
    'maps_test.cpp',
    'path_test.cpp',
    'point_test.cpp',
    'testrunner.cpp',
    'test_tools.cpp',
    'town_test.cpp',
    'complex_test.cpp',
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
    dependencies: [cppunit, boost]
    )

test('blieng test', bliengtest)
