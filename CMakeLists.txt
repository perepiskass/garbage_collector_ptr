# определяем минимальную версию для компиляции
cmake_minimum_required(VERSION 3.5)
project(garbage_collector CXX)
set(TARGET test_gcp)

# --------------------------------------------------------------------------
if(MSVC)
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest/googletest"
)
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
)
# --------------------------------------------------------------------------
set(HEADER_FILE_ONLY 
    "${CMAKE_SOURCE_DIR}/include/gcp.h"
    "${CMAKE_SOURCE_DIR}/include/info_unit.h"
    "${CMAKE_SOURCE_DIR}/include/iterator.h"
)
set(SOURCE_DIR
    "${CMAKE_SOURCE_DIR}/source/main.cpp"
)

add_executable(
    ${TARGET}
    ${SOURCE_DIR}
)

# --------------------------------------------------------------------------

target_include_directories(
    ${TARGET} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# УСТАНАВЛИВАЕМ СВОЙСТВА ДЛЯ КОМПИЛЯЦИИ ИСПОЛНЯЕМЫМ ФАЙЛАМ И БИБЛИОТЕКАМ----

set_target_properties(
    ${TARGET} PROPERTIES
# Стандарт С++ версии 17
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# СВОЙСТВА ДЛЯ КОМПИЛЯТОРОВ (ЛЮБЫЕ ПРЕДУПРЕЖДЕНИЯ ТРАКТОВАТЬ КАК ОШИБКИ)
if (MSVC)
    target_compile_options(${TARGET} PRIVATE
        /W4
    )
else ()
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
# ----------------------------------------------------------------------
enable_testing()
# --------------------------------------------------------------------------
