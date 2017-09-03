cmake_minimum_required(VERSION 2.8)
project(EXEPEEK)

set(CMAKE_SYSTEM_NAME Windows)

# recursively find all related files
file(GLOB_RECURSE PEPEEK_CPP *.cpp)

add_executable(exepeek WIN32 ${PEPEEK_CPP})

# set flags into compiler & linker
target_compile_options(exepeek PRIVATE -Os -pipe -Wall -Wp,-D_FORTIFY_SOURCE=2 -fexceptions --param=ssp-buffer-size=4)
target_link_libraries(exepeek comctl32 gdi32 -static)
