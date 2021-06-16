#ifndef LIBBITCOIN_THESIS_HD_PUBLIC_H
#define LIBBITCOIN_THESIS_HD_PUBLIC_H

#include <string>
#include <bitcoin/bitcoin.hpp>

#include "../utilities.h"
#include "hd_lineage.h"

class hd_private;

/**
 * BIP32 HD wallet public key
 */
class hd_public {
public:
    static const uint32_t mainnet;
    static const uint32_t testnet;

    static const uint32_t first_hardened_key = 1 << 31;

    hd_public();
    hd_public(const hd_private& priv);

    std::string encoded() const;

    hd_public derive_public(uint32_t index) const;

    const bc::byte_array<33>& get_point() const;

private:
    bc::byte_array<33> _point;
    hd_lineage _lineage;
    bc::byte_array<32> _chain_code;

    bool _valid = true;

    uint32_t fingerprint() const;

    hd_public(const bc::byte_array<33>& point, const hd_lineage& lineage,
              const bc::byte_array<32>& chain_code);
};


#endif //LIBBITCOIN_THESIS_HD_PUBLIC_H
