cmake_minimum_required(VERSION 3.7)
project(RoboUtils VERSION 0.1 LANGUAGES CXX)

include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 17)
file(GLOB SOURCES src/*.cpp src/io/*.cpp src/comm/*.cpp)

file(GLOB HEADERS include/*.h include/io/*.h include/comm/*.h include/chips/*.h)

add_library(libroboutils ${SOURCES})

target_include_directories(libroboutils PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE src)

install(TARGETS libroboutils EXPORT RoboUtilsConfig
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})  # This is for Windows

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT RoboUtilsConfig DESTINATION share/RoboUtils/cmake)

export(TARGETS libroboutils FILE RoboUtilsConfig.cmake)
