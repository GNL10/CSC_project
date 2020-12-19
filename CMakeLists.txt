cmake_minimum_required(VERSION 3.16)
project(SEALDemo VERSION 1.0)

link_libraries(stdc++fs)

find_package(SEAL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_executable(client Clients/client.cpp FHE.cpp)
target_link_libraries(client SEAL::seal)

add_executable(server Server/server.cpp)
target_link_libraries(server SEAL::seal)

add_executable(admin Admin/admin.cpp)
target_link_libraries(admin SEAL::seal)
