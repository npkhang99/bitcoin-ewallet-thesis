#include "utilities.h"

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
    bc::data_chunk ec_private;

//    raw.push_back(128);
//    bc::extend_data(raw, secret);
//    bc::append_checksum(raw);
//    std::cerr << "Uncompressed private key: " << bc::encode_base58(raw) << std::endl;

    ec_private.clear();
    ec_private.push_back(128);
    bc::extend_data(ec_private, secret);
    ec_private.push_back(1); // append 0x01 suffix in secret for compressed private key
    bc::append_checksum(ec_private);

    bc::data_chunk ec_public = generate_ec_pubkey_secp256k1(secret);

#ifdef DEBUG
    bc::wallet::ec_private control_private = bc::wallet::ec_private(secret);
    bc::wallet::ec_public control_pub = bc::wallet::ec_public(control_private);

    std::cerr << "Private key:" << std::endl;
    std::cerr << "   Got: " << bc::encode_base58(ec_private) << std::endl;
    std::cerr << "Expect: " << control_private.encoded() << std::endl;
    assert(bc::encode_base58(ec_private) == control_private.encoded());

    std::cerr << "Pubic key:" << std::endl;
    std::cerr << "   Got: " << bc::encode_base16(ec_public) << std::endl;
    std::cerr << "Expect: " << control_pub.encoded() << std::endl;
    assert(bc::encode_base16(ec_public) == control_pub.encoded());
#endif
    return {ec_public, ec_private};
}

std::string generate_address(const bc::data_chunk &public_key_data) {
    // bc::bitcoin_short_hash = bc::ripemd160_hash(bc::sha256_hash(data))
    bc::short_hash public_key_hash = bc::bitcoin_short_hash(public_key_data);

    // base58 check encode
    bc::data_chunk raw;
    raw.push_back(0); // version 0: bitcoin address
    bc::extend_data(raw, public_key_hash); // append public key hash
    bc::append_checksum(raw); // calculate and append 4 bytes checksum

    std::string bitcoin_address = bc::encode_base58(raw);

#ifdef DEBUG
    // validate with library's function
    bc::wallet::payment_address control =
            bc::wallet::payment_address(public_key_hash);
    assert(control.encoded() == bitcoin_address);
#endif

    return bitcoin_address;
}

bc::data_chunk generate_ec_pubkey_secp256k1(const bc::ec_secret &secret) {
    secp256k1_context *context = secp256k1_context_create(
            SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);
    secp256k1_pubkey pub_raw;

    bool success = secp256k1_ec_pubkey_create(context, &pub_raw, secret.data());
    if (!success) {
        secp256k1_context_destroy(context);
        throw std::runtime_error("Error creating pub_raw");
    }

    std::uint8_t serialized_pub[33];
    std::size_t len = sizeof(serialized_pub);
    secp256k1_ec_pubkey_serialize(context, serialized_pub, &len, &pub_raw,
                                  SECP256K1_EC_COMPRESSED);
    bc::data_chunk pub_chunk(serialized_pub, serialized_pub + len);
    secp256k1_context_destroy(context);

#ifdef DEBUG
    std::string expect = bc::encode_base16(
            bc::wallet::ec_public(bc::wallet::ec_private(secret)).point());
    assert(expect == bc::encode_base16(pub_chunk));
#endif

    return pub_chunk;
}
