#include "hd_private.h"

const uint32_t hd_private::mainnet = 76067358;
const uint32_t hd_private::testnet = 70617039;

hd_private::hd_private(const bc::data_chunk& seed, uint32_t prefix) {
    // BIP32 magic key for HMAC-SHA512
    const auto magic = bc::to_chunk("Bitcoin seed");

    const auto hash_output = bc::hmac_sha512_hash(seed, magic);
    const auto split = bc::split(hash_output);

    if (!bc::verify(split.left)) {
        throw std::range_error("Master key is invalid (is 0 or >= n)");
    }

    _secret = split.left;
    _chain_code = split.right;
}

const bc::byte_array<32>& hd_private::secret() const {
    return _secret;
}
