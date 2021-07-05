#include "client_utilities.h"

bool client::get_balance(std::string& out, const std::string& address,
                         const std::string& network) {
    std::string url =
            "https://chain.so/api/v2/address/" + network + "/" + address;

    std::stringstream ss(http_client(url).execute());
    Json::Value response;
    ss >> response;

    if (response["status"] != "success") {
        std::cerr << response["message"].asString() << std::endl;
        return false;
    }

    out = response["data"]["balance"].asString();

    return true;
}

bool client::get_tx_info(Json::Value& out, const std::string& txid,
                         const std::string& network) {
    std::string url = "https://chain.so/api/v2/get_tx/" + network + "/" + txid;

    std::stringstream ss(http_client(url).execute());
    Json::Value response;
    ss >> response;

    if (response["status"] != "success") {
        std::cerr << response["message"].asString() << std::endl;
        return false;
    }

    out = response["data"];

    return true;
}

bool client::get_address_info(Json::Value& out, const std::string& address,
                              const std::string& network) {
    std::string url =
            "https://chain.so/api/v2/address/" + network + "/" + address;

    std::stringstream ss(http_client(url).execute());
    Json::Value response;
    ss >> response;

    if (response["status"] != "success") {
        std::cerr << response["message"].asString() << std::endl;
        return false;
    }

    out = response["data"];

    return true;
}
