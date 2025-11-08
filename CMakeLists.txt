cmake_minimum_required (VERSION 4.0.0)

project ("STC_Project")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory ("ByteProcess") # Source code
add_subdirectory ("Testing") # Testing
