#ifndef LIBBITCOIN_THESIS_HD_PRIVATE_H
#define LIBBITCOIN_THESIS_HD_PRIVATE_H

#include <string>
#include <bitcoin/bitcoin.hpp>

#include "hd_lineage.h"
#include "hd_public.h"
#include "wallet_utilities.h"

/**
 * BIP32 HD wallet private key
 */
class hd_private {
public:
    static const uint32_t mainnet;
    static const uint32_t testnet;

    static const uint32_t first_hardened_key = 1 << 31;

    hd_private();
    hd_private(const hd_private& o);
    hd_private(const bc::data_chunk& seed, uint32_t version = mainnet);

    std::string encoded() const;

    hd_private derive_private(uint32_t index) const;
    hd_public derive_public(uint32_t index) const;

    hd_public to_public() const;

    const bc::byte_array<32>& get_secret() const;
    const bc::byte_array<32>& get_chain() const;
    const hd_lineage& get_lineage() const;

private:
    bc::byte_array<32> _secret;
    hd_lineage _lineage = {};
    bc::byte_array<32> _chain_code;

    uint32_t fingerprint() const;

    hd_private(const bc::byte_array<32>& secret, const hd_lineage& lineage,
               const bc::byte_array<32>& chain_code);
};


#endif //LIBBITCOIN_THESIS_HD_PRIVATE_H
