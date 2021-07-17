cmake_minimum_required(VERSION 3.0)
project(bitcoin-ewallet-thesis)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -DWITH_PNG -DWITH_QRENCODE -DWITH_ICU")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")

find_package(PkgConfig REQUIRED)
find_package(oatpp REQUIRED)
pkg_check_modules(BITCOIN REQUIRED libbitcoin)
pkg_check_modules(CURL REQUIRED libcurl)
pkg_check_modules(JSON REQUIRED jsoncpp)

add_executable(
        bitcoin-ewallet-thesis

        src/main.cpp
        src/tests.cpp src/tests.h

        src/interactive/data.h
        src/interactive/tdata.h
        src/interactive/utilities.cpp src/interactive/utilities.h
        src/interactive/interactive_shell.cpp src/interactive/interactive_shell.h
        src/interactive/transaction_builder.cpp src/interactive/transaction_builder.h

        src/wallet/wallet_utilities.cpp src/wallet/wallet_utilities.h
        src/wallet/hd_wallet.cpp src/wallet/hd_wallet.h
        src/wallet/hd_private.cpp src/wallet/hd_private.h
        src/wallet/hd_public.cpp src/wallet/hd_public.h
        src/wallet/hd_lineage.h
        src/wallet/payment_address.cpp src/wallet/payment_address.h
        src/wallet/trans_info.h

        src/chain/transaction.cpp src/chain/transaction.h
        src/chain/input.cpp src/chain/input.h
        src/chain/output.cpp src/chain/output.h
        src/chain/chain_utilities.cpp src/chain/chain_utilities.h

        src/client/http_client.cpp src/client/http_client.h
        src/client/client_utilities.cpp src/client/client_utilities.h

        src/server/http_server.cpp src/server/http_server.h
        src/server/dto/dtos.h
        src/server/components.h
        src/server/controller/controller.h
)

target_link_libraries(bitcoin-ewallet-thesis
        ${BITCOIN_LIBRARIES}
        ${CURL_LIBRARIES}
        ${JSON_LIBRARIES}
        oatpp::oatpp
)
target_link_directories(bitcoin-ewallet-thesis PUBLIC ${BITCOIN_LIBRARY_DIRS})
target_include_directories(bitcoin-ewallet-thesis PUBLIC ${BITCOIN_INCLUDE_DIRS})
target_compile_options(bitcoin-ewallet-thesis PUBLIC ${BITCOIN_CFLAGS_OTHER})
