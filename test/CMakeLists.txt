cmake_minimum_required(VERSION 3.8)

set(TARGET test_list)

add_executable(
    ${TARGET}
    "${CMAKE_CURRENT_SOURCE_DIR}/gtest.cpp"
)

target_include_directories(
    ${TARGET} PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries( 
    ${TARGET} PUBLIC
    gtest
    gtest_main
    gmock
    gmock_main
    # libgcp
)

set_target_properties(
    ${TARGET} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(
        ${TARGET} PRIVATE
        /W4
    )
else ()
    target_compile_options(
        ${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

add_test(
    NAME ${TARGET}
    COMMAND ${TARGET}
)