cmake_minimum_required(VERSION 3.0)
project(libbitcoin-thesis)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DWITH_PNG -DWITH_QRENCODE -DWITH_ICU")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

include_directories(/usr/local/include)

add_executable(libbitcoin-thesis src/main.cpp src/functions.cpp src/hd_wallet.cpp src/hd_wallet.h)
target_link_libraries(libbitcoin-thesis PUBLIC /usr/local/lib/libbitcoin.so)
