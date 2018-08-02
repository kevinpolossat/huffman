CMAKE_MINIMUM_REQUIRED(VERSION 3.2.1)
project(rtype CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(huffman src/main.cc)