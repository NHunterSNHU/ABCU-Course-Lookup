cmake_minimum_required(VERSION 4.2)
project(ABCU_Course_Lookup)
# Force static linking for MinGW/GCC compilers to avoid missing DLL errors
if(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

set(CMAKE_CXX_STANDARD 20)

add_executable(ABCU_Course_Lookup ProjectTwo.cpp)
