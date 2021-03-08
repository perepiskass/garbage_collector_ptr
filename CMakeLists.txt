# определяем минимальную версию для компиляции
cmake_minimum_required(VERSION 3.5)
project(garbage_collector VERSION 1.1.5)

# --------------------------------------------------------------------------
set(TARGET test)
add_executable(${TARGET} ${CMAKE_SOURCE_DIR}/source/main.cpp)

# --------------------------------------------------------------------------

target_include_directories(${TARGET} PRIVATE
${CMAKE_SOURCE_DIR}/include)

# УСТАНАВЛИВАЕМ СВОЙСТВА ДЛЯ КОМПИЛЯЦИИ ИСПОЛНЯЕМЫМ ФАЙЛАМ И БИБЛИОТЕКАМ----

set_target_properties(${TARGET} PROPERTIES
# Стандарт С++ версии 17
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