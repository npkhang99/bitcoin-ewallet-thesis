#ifndef LIBBITCOIN_THESIS_PAYMENT_ADDRESS_H
#define LIBBITCOIN_THESIS_PAYMENT_ADDRESS_H

#include <cstdint>
#include <string>
#include <stdexcept>
#include <bitcoin/bitcoin.hpp>

#include "hd_public.h"
#include "hd_private.h"

/**
 * Bitcoin
 */
class payment_address {
public:
    static const uint8_t mainnet_p2kh;
    static const uint8_t testnet_p2kh;

    payment_address();
    payment_address(const payment_address& o);
    payment_address(const hd_public& pubkey, uint8_t version = mainnet_p2kh);
    payment_address(const hd_private& privkey, uint8_t version = mainnet_p2kh);
    payment_address(const bc::byte_array<33>& point, uint8_t version = mainnet_p2kh);

    /**
     * Base58Check encode bitcoin address
     * @return std::string the encoded address
     */
    std::string encoded() const;

private:
    uint8_t _version;
    bc::short_hash _hash;
};


#endif //LIBBITCOIN_THESIS_PAYMENT_ADDRESS_H
