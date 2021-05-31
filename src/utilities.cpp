#include "utilities.h"

bc::ec_secret generate_secret(const std::string& secret_hex = "") {
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

ec_key_pair generate_wallet_ec_key_pair(const bc::ec_secret& secret) {
    bc::data_chunk ec_private;

    ec_private.push_back(128);
    bc::extend_data(ec_private, secret);
    ec_private.push_back(1); // compressed private key
    bc::append_checksum(ec_private);

    bc::byte_array<33> ec_public = secp256k1_point(secret);

#ifdef DEBUG
    bc::wallet::ec_private control_private = bc::wallet::ec_private(secret);
    bc::wallet::ec_public control_pub = bc::wallet::ec_public(control_private);

    assert(bc::encode_base58(ec_private) == control_private.encoded());
    assert(bc::encode_base16(ec_public) == control_pub.encoded());
#endif

    return {ec_public, ec_private};
}

bc::byte_array<33> secp256k1_point(const bc::ec_secret& secret) {
    // secp256k1 generator point
    auto gen_point = bc::base16_literal(
            "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798");

    bc::byte_array<33> pub_key(gen_point);
    bc::ec_multiply(pub_key, secret);

#ifdef DEBUG
    std::string expect = bc::encode_base16(
            bc::wallet::ec_public(bc::wallet::ec_private(secret)).point());
    assert(expect == bc::encode_base16(pub_key));
#endif

    return pub_key;
}
