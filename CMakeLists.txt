cmake_minimum_required(VERSION 2.8)
project(manjaro-web-repo)

SET (CMAKE_INSTALL_PREFIX "/usr/")
SET (WT_CONNECTOR "wthttp" CACHE STRING "Connector used (wthttp or wtfcgi)")

add_definitions(-std=c++11 -Wall)

add_subdirectory(src)
add_subdirectory(data)
