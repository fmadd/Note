cmake_minimum_required(VERSION 3.26)
project(NoteServer)

set(CMAKE_CXX_STANDARD 20)
add_executable(NoteServer
        src/main.cpp
        src/base.cpp
        src/server.hpp
        src/crypto.hpp
)
find_package(Boost REQUIRED)
#
#add_executable(tests
#        tests/db_test.cpp
#        src/base.cpp
#        src/server.hpp
#)

add_executable(gtests
        tests/server_test.cpp
        src/crypto.hpp
        src/base.cpp
        src/server.hpp
)
add_test(NAME tests COMMAND tests --force-colors -d)

IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    #target_link_libraries(NoteServer ws2_32)
    target_link_libraries (NoteServer ${BOOST_LIBRARIES})
    ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_subdirectory(libs/libpqxx build-pqxx)
add_subdirectory(external/doctest)

target_link_libraries(${PROJECT_NAME}  PRIVATE pqxx cryptopp  doctest)
#target_link_libraries(tests  PUBLIC pqxx doctest cryptopp gtest gmock)
target_link_libraries(gtests  PUBLIC pqxx cryptopp gtest gmock)