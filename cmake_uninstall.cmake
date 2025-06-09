# CMake uninstall script
# Usage: cmake -P cmake_uninstall.cmake
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
    set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
message(STATUS "Uninstalling diff-numerics and man page from ${CMAKE_INSTALL_PREFIX}")
file(REMOVE "${CMAKE_INSTALL_PREFIX}/bin/diff-numerics")
file(REMOVE "${CMAKE_INSTALL_PREFIX}/share/man/man1/diff-numerics.1")
execute_process(COMMAND mandb)
message(STATUS "Uninstall complete.")
