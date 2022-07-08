cmake_minimum_required(VERSION 3.22)
project(PasswordDecryption)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fopenmp -pthread")

add_executable(PasswordDecryption main.cpp includes/word_generator.h includes/sequential_decryption.h includes/pthread_decryption.h includes/omp_decryption.h)
target_link_libraries(PasswordDecryption -lcrypt)


