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
    bc::data_chunk raw;

//    raw.push_back(128);
//    bc::extend_data(raw, secret);
//    bc::append_checksum(raw);
//    std::cerr << "Uncompressed private key: " << bc::encode_base58(raw) << std::endl;

//    raw.clear();
//    raw.push_back(128);
//    bc::extend_data(raw, secret);
//    raw.push_back(01); // append 0x01 suffix in secret for compressed private key
//    bc::append_checksum(raw);
//    std::cerr << "Compressed private key: " << bc::encode_base58(raw) << std::endl;

    // default will be compressed private key (prefix will be K or L)
    bc::wallet::ec_private ec_private = bc::wallet::ec_private(secret);
    bc::wallet::ec_public ec_public = bc::wallet::ec_public(ec_private);
    return {ec_public, ec_private};
}

std::string generate_address(const ec_key_pair &key_pair) {
    bc::wallet::ec_public public_key = key_pair.first;
    bc::data_chunk public_key_data;
    public_key.to_data(public_key_data);

    // bc::bitcoin_short_hash = bc::ripemd160_hash(bc::sha256_hash(data))
    bc::short_hash public_key_hash = bc::bitcoin_short_hash(public_key_data);

    // base58 check encode
    bc::data_chunk raw;
    raw.push_back(0); // version 0: bitcoin address
    bc::extend_data(raw, public_key_hash); // append public key hash
    bc::append_checksum(raw); // calculate and append 4 bytes checksum

    std::string bitcoin_address = bc::encode_base58(raw);

    // validate with library's function
    bc::wallet::payment_address control =
            bc::wallet::payment_address(public_key_hash);
    assert(control.encoded() == bitcoin_address);

    return control.encoded();
}
