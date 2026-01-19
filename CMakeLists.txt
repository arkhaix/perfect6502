cmake_minimum_required(VERSION 3.28)
project(perfect6502 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMANDS ON)

add_library(perfect6502 STATIC
  netlist_6502.h
  netlist_sim.c
  netlist_sim.h
  perfect6502.c
  perfect6502.h
  types.h
)

add_executable(cbmbasic
    cbmbasic/cbmbasic.c
    cbmbasic/runtime.c
    cbmbasic/runtime_init.c
    cbmbasic/plugin.c
    cbmbasic/console.c
    cbmbasic/emu.c
    cbmbasic/console.h
    cbmbasic/glue.h
    cbmbasic/plugin.h
    cbmbasic/readdir.h
    cbmbasic/runtime.h
    cbmbasic/runtime_init.h
    cbmbasic/stat.h
)
target_include_directories(cbmbasic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cbmbasic PRIVATE perfect6502)

add_executable(apple1basic
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    apple1basic/apple1basic.c
)
target_include_directories(apple1basic PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(apple1basic PRIVATE perfect6502)

add_executable(measure
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    measure.c
)
target_include_directories(measure PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(measure PRIVATE perfect6502)

include(FetchContent)
FetchContent_Declare(
  gflags
  GIT_REPOSITORY https://github.com/gflags/gflags
  GIT_TAG v2.3.0
)
FetchContent_MakeAvailable(gflags)

add_executable(ark
  ark/main.cpp
  ark/disasm.cpp
  ark/disasm.h
)
target_include_directories(ark PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ark PRIVATE perfect6502 gflags)

option(USE_CURSES "Enable ncurses support" ON)
if(USE_CURSES)
  find_package(Curses QUIET)
  if(CURSES_FOUND)
    message(STATUS "Building with ncurses support enabled")
    target_compile_definitions(ark PRIVATE WITH_CURSES)
    target_include_directories(ark PRIVATE ${CURSES_INCLUDE_DIRS})
    target_link_libraries(ark PRIVATE ${CURSES_LIBRARIES})
  endif()
endif()


