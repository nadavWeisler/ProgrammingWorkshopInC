cmake_minimum_required(VERSION 3.16)
project(Exam C)

set(CMAKE_C_STANDARD 99)

add_executable(Exam SpreaderDetectorBackend.c SpreaderDetectorParams.h)