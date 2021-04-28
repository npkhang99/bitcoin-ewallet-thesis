#include "functions.h"

bc::ec_secret generate_secret(const std::string &secret_hex = "") {
    bc::ec_secret secret_key;

    if (secret_hex.length() == 0) {
        bc::data_chunk seed(16);
        bc::pseudo_random_fill(seed);
        secret_key = bc::bitcoin_hash(seed);
    } else {
        bc::decode_base16(secret_key, secret_hex);
    }

    return secret_key;
}

ec_key_pair generate_wallet_ec_key_pair(bc::ec_secret secret) {
    bc::wallet::ec_private ec_private = bc::wallet::ec_private(secret);
    bc::wallet::ec_public ec_public = bc::wallet::ec_public(ec_private);
    return {ec_public, ec_private};
}

std::string base58_check_encode(const bc::short_hash &payload) {
    bc::data_chunk raw;
    raw.push_back(0); // add version
    bc::extend_data(raw, payload); // append public key hash to raw
    bc::append_checksum(raw); // auto calculate & append 4 bytes checksum to raw
    return bc::encode_base58(raw); // base 58 encode
}

std::string generate_address(const ec_key_pair &key_pair) {
    bc::wallet::ec_public public_key = key_pair.first;
    bc::data_chunk public_key_data;
    public_key.to_data(public_key_data);

    // bc::bitcoin_short_hash = bc::ripemd160_hash(bc::sha256_hash(data))
    bc::short_hash public_key_hash = bc::bitcoin_short_hash(public_key_data);

    std::string bitcoin_address = base58_check_encode(public_key_hash);

    // check correctness
    bc::wallet::payment_address control =
            bc::wallet::payment_address(public_key_hash);
    assert(control.encoded() == bitcoin_address);

    return control.encoded();
}
