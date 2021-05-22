cmake_minimum_required(VERSION 3.0)
project(libbitcoin-thesis)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DWITH_PNG -DWITH_QRENCODE -DWITH_ICU")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(PkgConfig REQUIRED)
pkg_check_modules(BITCOIN REQUIRED libbitcoin)

add_executable(
        libbitcoin-thesis
        src/main.cpp
        src/utilities.cpp src/utilities.h
        src/hd_wallet.cpp src/hd_wallet.h
        src/hd_private.cpp src/hd_private.h
        src/tests.cpp src/tests.h
        src/hd_public.cpp src/hd_public.h
        src/hd_lineage.h)

target_link_libraries(libbitcoin-thesis ${BITCOIN_LIBRARIES})
target_link_directories(libbitcoin-thesis PUBLIC ${BITCOIN_LIBRARY_DIRS})
target_include_directories(libbitcoin-thesis PUBLIC ${BITCOIN_INCLUDE_DIRS})
target_compile_options(libbitcoin-thesis PUBLIC ${BITCOIN_CFLAGS_OTHER})
