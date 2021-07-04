#ifndef LIBBITCOIN_THESIS_CLIENT_UTILITIES_H
#define LIBBITCOIN_THESIS_CLIENT_UTILITIES_H

#include <string>
#include <sstream>

#include <json/json.h>

#include "http_client.h"

namespace client {
    const std::string mainnet = "BTC";
    const std::string testnet = "BTCTEST";

    /**
     * Get balance of a given bitcoin address
     * @param out the balance in BTC
     * @param address the encoded address
     * @param url the network url to connect
     * @return true if success, otherwise false
     */
    bool get_balance(std::string& out, const std::string& address,
                     const std::string& network = mainnet);

    /**
     * Get transaction info
     * @param out the json of the tx info
     * @param txid the transaction hash
     * @param network mainnet or testnet
     * @return true if success, otherwise false
     */
    bool get_tx_info(Json::Value& out, const std::string& txid,
                     const std::string& network = mainnet);

    /**
     * Get address info
     * @param out the json of the assdress info
     * @param address the encoded address
     * @param network mainnet or testnet
     * @return true if success, otherwise false
     */
    bool get_address_info(Json::Value& out,
                          const std::string& address,
                          const std::string& network = mainnet);
}

#endif //LIBBITCOIN_THESIS_CLIENT_UTILITIES_H
