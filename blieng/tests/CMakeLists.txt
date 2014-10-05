set(CMAKE_CXX_FLAGS "-std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REENTRANT")

include(ExternalProject)
ExternalProject_Add(
    GoogleMock
    PREFIX ${CMAKE_BINARY_DIR}/GoogleMock
    URL http://googlemock.googlecode.com/files/gmock-1.6.0.zip
    URL_MD5 f547f47321ca88d3965ca2efdcc2a3c1
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY:PATH=${CMAKE_BINARY_DIR}/libs/gmock
            -DCMAKE_LIBRARY_OUTPUT_DIRECTORY:PATH=${CMAKE_BINARY_DIR}/libs/gmock
            -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    INSTALL_COMMAND ""
)
set(GMOCK_DIR "${CMAKE_BINARY_DIR}/GoogleMock/src/GoogleMock")
set(GMOCK_INCLUDE_DIR "${GMOCK_DIR}/include")
set(GMOCK_LIB_DIR "${CMAKE_BINARY_DIR}/libs/gmock")

include_directories(${GMOCK_INCLUDE_DIR})
include_directories(${GMOCK_DIR}/gtest/include)
include_directories(..)
include_directories(../..)

add_executable(blieng_test
    auto_map_test.cpp
    auto_vector_test.cpp
    bliany_test.cpp
    bliobject_test.cpp
    card_test.cpp
    character_test.cpp
    configure_test.cpp
    datafile_test.cpp
    data_test.cpp
    eventlog_test.cpp
    item_test.cpp
    logging_test.cpp
    maps_test.cpp
    path_test.cpp
    point_test.cpp
    testrunner.cpp
    test_tools.cpp
    town_test.cpp
    auto_map_test.h
    auto_vector_test.h
    bliany_test.h
    bliobject_test.h
    card_test.h
    character_test.h
    configure_test.h
    datafile_test.h
    data_test.h
    eventlog_test.h
    item_test.h
    logging_test.h
    maps_test.h
    path_test.h
    point_test.h
    test_tools.h
    town_test.h
    )

add_dependencies(blieng_test GoogleMock)

target_link_libraries(blieng_test
    cppunit
    dl
    blieng
    pthread)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})
target_link_libraries(blieng_test ${Boost_LIBRARIES})

target_link_libraries(blieng_test
    ${GMOCK_LIB_DIR}/libgmock.a
    ${GMOCK_LIB_DIR}/libgtest.a
    )
