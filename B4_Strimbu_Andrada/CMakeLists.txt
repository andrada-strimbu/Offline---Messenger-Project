cmake_minimum_required(VERSION 3.24)
project(untitled2)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server.cpp )
add_executable(client client.cpp )
add_executable(client1 client1.cpp )
target_link_libraries(server pthread)
target_link_libraries ( server -ljsoncpp)

