cmake_minimum_required(VERSION 2.8.8)

project(blieng)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

find_package(Qt5Core REQUIRED)

add_subdirectory(blieng)

enable_testing()
add_test(blieng_unittest
    blieng/tests/blieng_test)

set(OUT blieng-alpha4)
set(DISTEXT tar.gz)


if (NOT DEFINED BITS)
    set(BITS 32bit)
endif()
if (NOT DEFINED TRACK)
    set(TRACK dev)
endif()

add_custom_target(dist
    COMMAND mkdir -p dist/${OUT}
    COMMAND mkdir -p dist/${OUT}/include
    COMMAND cp "$<TARGET_FILE:blieng>" dist/${OUT}
    COMMAND cp "${CMAKE_CURRENT_LIST_DIR}/blieng/*.h" "dist/${OUT}/include"
    COMMAND cd dist && tar czf ${OUT}.${DISTEXT} ${OUT}
    DEPENDS blieng
    )

add_custom_target(upload
    COMMAND echo "${CMAKE_CURRENT_LIST_DIR}/tools/build/binrep_upload.sh" dist/${OUT}.${DISTEXT} blieng ${BITS} ${TRACK}
    DEPENDS dist
    )
