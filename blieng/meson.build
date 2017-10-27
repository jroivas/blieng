blieng_lib = static_library('blieng',
    'blieng.cpp',
    'card.cpp',
    'character.cpp',
    'configure.cpp',
    'eventlog.cpp',
    'item.cpp',
    'logging.cpp',
    'maps.cpp',
    'path.cpp',
    'point.cpp',
    'town.cpp',
    'wallclock.cpp',
    'data.cpp',
    'datafile.cpp',
    'bliobject.cpp',
    'complex.cpp',
    'fastlz.c',
    'rijndael-alg-fst.c',
    'json.c',
    'blidynamicbody.cpp',
    'bliworld.cpp',
    cpp_args : ['-std=c++11'],
    pic : true,
    include_directories : [
        box2d.get_variable('toplevel_inc'),
        blieng_inc,
        toplevel_inc
    ],
    link_with: [
        box2d.get_variable('box2d_lib')
    ]
)
