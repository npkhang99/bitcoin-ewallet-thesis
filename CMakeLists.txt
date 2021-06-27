cmake_minimum_required(VERSION 3.0)
project(libbitcoin-thesis)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DWITH_PNG -DWITH_QRENCODE -DWITH_ICU")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(PkgConfig REQUIRED)
pkg_check_modules(BITCOIN REQUIRED libbitcoin)
pkg_check_modules(BITCOIN_CLIENT REQUIRED libbitcoin-client)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(JSON REQUIRED jsoncpp)

add_executable(
        libbitcoin-thesis
        src/main.cpp
        src/tests.cpp src/tests.h
        src/utilities.cpp src/utilities.h

        src/wallet/hd_wallet.cpp src/wallet/hd_wallet.h
        src/wallet/hd_private.cpp src/wallet/hd_private.h
        src/wallet/hd_public.cpp src/wallet/hd_public.h
        src/wallet/hd_lineage.h
        src/wallet/payment_address.cpp src/wallet/payment_address.h

        src/chain/transaction.cpp src/chain/transaction.h
        src/chain/input.cpp src/chain/input.h
        src/chain/output.cpp src/chain/output.h
        src/chain/chain_utilities.cpp src/chain/chain_utilities.h)

target_link_libraries(libbitcoin-thesis ${BITCOIN_LIBRARIES} ${BITCOIN_CLIENT_LIBRARIES} ${CURL_LIBRARIES} ${JSON_LIBRARIES})
target_link_directories(libbitcoin-thesis PUBLIC ${BITCOIN_LIBRARY_DIRS} ${BITCOIN_CLIENT_LIBRARY_DIRS})
target_include_directories(libbitcoin-thesis PUBLIC ${BITCOIN_INCLUDE_DIRS} ${BITCOIN_CLIENT_INCLUDE_DIRS})
target_compile_options(libbitcoin-thesis PUBLIC ${BITCOIN_CFLAGS_OTHER} ${BITCOIN_CLIENT_CFLAGS_OTHER})
