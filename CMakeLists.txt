cmake_minimum_required(VERSION 3.8)
project("Si-Paling-Cerdas" LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
add_compile_definitions(SUPPORT_FILEFORMAT_JPG)

add_subdirectory(src)
add_subdirectory(vendor)
