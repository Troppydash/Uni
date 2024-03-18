cmake_minimum_required(VERSION 3.27)
project(ass1 C)

set(CMAKE_C_STANDARD 11)


add_executable(ass1 convexHull.c convexHull.h linkedList.c linkedList.h problem.c problem.h stack.c stack.h
#        problem1a.c
        test.c
        #        problem1b.c
)
target_link_libraries(ass1 m)
