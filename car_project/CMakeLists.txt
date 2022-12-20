cmake_minimum_required(VERSION 3.24)
project(car_project C)

set(CMAKE_C_STANDARD 11)

add_executable(car_project main.c auto_scanner.c auto_scanner.h controller.c controller.h main.h pthread_usr.c pthread_usr.h REG.h serial.c serial.h wit_c_sdk.c wit_c_sdk.h serial_chassic.cpp serial_chassic.h socket_connection.cpp socket_connection.h)

find_package(Threads)

target_link_libraries(car_project ${CMAKE_THREAD_LIBS_INIT})