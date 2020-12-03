cmake_minimum_required(VERSION 3.16)
project(SEALDemo VERSION 1.0)

find_package(SEAL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

add_executable(FHE FHE.cpp)
target_link_libraries(FHE SEAL::seal)

add_executable(SEALDemo SEALDemo.cpp)
target_link_libraries(SEALDemo SEAL::seal)