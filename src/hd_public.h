#ifndef LIBBITCOIN_THESIS_HD_PUBLIC_H
#define LIBBITCOIN_THESIS_HD_PUBLIC_H

#include <string>
#include <bitcoin/bitcoin.hpp>

#include "utilities.h"
#include "hd_private.h"

/**
 * BIP32 HD wallet public key
 */
class hd_public {
public:
    static const uint32_t mainnet;
    static const uint32_t testnet;

    hd_public(const hd_private& priv, uint32_t prefix = mainnet);

    std::string to_payment_address() const;

    const bc::byte_array<33>& get_point() const;

private:
    bc::byte_array<33> _point;
    bc::byte_array<32> _chain_code;
};


#endif //LIBBITCOIN_THESIS_HD_PUBLIC_H
