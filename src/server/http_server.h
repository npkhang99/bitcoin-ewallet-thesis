#ifndef BITCOIN_EWALLET_THESIS_HTTP_SERVER_H
#define BITCOIN_EWALLET_THESIS_HTTP_SERVER_H

#include <cstdint>
#include <string>

#include <oatpp/web/server/HttpConnectionHandler.hpp>
#include <oatpp/network/Server.hpp>
#include <oatpp/network/tcp/server/ConnectionProvider.hpp>

#include "data.h"
#include "components.h"
#include "controller/controller.h"

#include "../wallet/hd_wallet.h"

using namespace oatpp;

class http_server {
public:
    http_server();
    http_server(const std::string& app_name);

    void run();

private:
    void server_run();
};


#endif //BITCOIN_EWALLET_THESIS_HTTP_SERVER_H
