cmake_minimum_required(VERSION 3.0)
project(libbitcoin-thesis)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DWITH_PNG -DWITH_QRENCODE")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

link_directories(
        /usr/local/include
        /usr/include/libpng16
        /usr/local/lib
        /usr/lib64
)

link_libraries(
        bitcoin
        boost_chrono
        boost_date_time
        boost_filesystem
        boost_iostreams
        boost_locale
        boost_log
        boost_program_options
        boost_regex
        boost_system
        boost_thread
        pthread
        rt
        dl
        png16
        z
        qrencode
        secp256k1
)

add_executable(libbitcoin-thesis src/main.cpp src/functions.cpp src/hd_wallet.cpp src/hd_wallet.h)
