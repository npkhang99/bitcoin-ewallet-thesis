#include "hd_public.h"

const uint32_t hd_public::mainnet = 76067358;
const uint32_t hd_public::testnet = 70617039;

hd_public::hd_public(const hd_private& priv, uint32_t prefix) {
    _point = generate_ec_pubkey_secp256k1(priv.get_secret());
    _chain_code = priv.get_chain();
}

std::string hd_public::to_payment_address() const {
    return generate_address(_point);
}

const bc::byte_array<33>& hd_public::get_point() const {
    return _point;
}
