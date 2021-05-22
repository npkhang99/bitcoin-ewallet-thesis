#ifndef LIBBITCOIN_THESIS_HD_PRIVATE_H
#define LIBBITCOIN_THESIS_HD_PRIVATE_H

#include <string>
#include <bitcoin/bitcoin.hpp>

/**
 * BIP32 HD wallet private key
 */
class hd_private {
public:
    static const uint32_t mainnet;
    static const uint32_t testnet;

    hd_private(const bc::data_chunk& seed, uint32_t prefix = mainnet);

    const bc::byte_array<32>& secret() const;

private:
    bc::byte_array<32> _secret;
    bc::byte_array<32> _chain_code;
};


#endif //LIBBITCOIN_THESIS_HD_PRIVATE_H
