add_library(blieng
    blieng.cpp
    card.cpp
    character.cpp
    configure.cpp
    eventlog.cpp
    item.cpp
    logging.cpp
    maps.cpp
    path.cpp
    point.cpp
    town.cpp
    wallclock.cpp
    data.cpp
    datafile.cpp
    bliobject.cpp
    auto_vector.h
    bliany.h
    blieng.h
    card.h
    character.h
    config.h
    eventlog.h
    fastlz.h
    item.h
    logging.h
    maps.h
    path.h
    point.h
    rijndael-alg-fst.h
    town.h
    wallclock.h
    auto_map.h
    data.h
    datafile.h
    json.h
    bliobject.h
    configure.h
    fastlz.c
    rijndael-alg-fst.c
    json.c
    )

add_dependencies(blieng
    fetch_tools_build)

qt5_use_modules(blieng Core)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -pedantic -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-declarations -Wmissing-include-dirs -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-promo -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wall -Wextra -Wno-error=type-limits -Wno-error=sign-compare")

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost 1.47
    COMPONENTS
    date_time
    system
    filesystem
    random
    serialization)
include_directories(${Boost_INCLUDE_DIR})
include_directories(..)

add_subdirectory(tests)
