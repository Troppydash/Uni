cmake_minimum_required(VERSION 3.27)
project(ass1b C)

set(CMAKE_C_STANDARD 11)

add_executable(ass1b
        list.c
        list.h
        graph.c
        graph.h
        pq.c
        pq.h
        utils.c
        utils.h
        problem2a.c
#        problem2b.c
#        problem2c.c
#        problem2d.c
)
