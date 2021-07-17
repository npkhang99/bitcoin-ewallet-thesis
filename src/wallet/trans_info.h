#ifndef LIBBITCOIN_THESIS_TRANS_INFO_H
#define LIBBITCOIN_THESIS_TRANS_INFO_H

#include <string>
#include <bitcoin/bitcoin.hpp>

struct trans_info {
    hd_private priv;
    bc::chain::output_point previous_tx;
    uint64_t value;

    std::string to_string() const {
        std::string str;
        str = "TXID: " + bc::encode_hash(previous_tx.hash()) + " | " +
              "Value: " + bc::encode_base10(value, 8) + " BTC"
#ifdef DEBUG
              + " | Address: " + payment_address(priv).encoded()
#endif
              ;
        return str;
    }
};

#endif //LIBBITCOIN_THESIS_TRANS_INFO_H
