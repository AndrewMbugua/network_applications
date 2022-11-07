cmake_minimum_required(VERSION 3.22.1)
project(dns_query C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(dns_query
        headers.h
        main.c
        ip_lookup.c)
