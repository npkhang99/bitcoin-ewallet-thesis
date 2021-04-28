#include <iostream>
#include <string>
#include <functional>

#include <bitcoin/bitcoin.hpp>

typedef std::pair<bc::wallet::ec_public, bc::wallet::ec_private> ec_key_pair;

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

int main() {
    bc::ec_secret secret = generate_secret("067375cf2f49e9bc7b759acdcc6b9cd43326a550415eff5ba8d5e266d7adfca3");
    std::cout << "Secret hex: " << std::endl << bc::encode_base16(secret) << std::endl;

    auto key_pair = generate_wallet_ec_key_pair(secret);
    std::cout << "EC private key: " << std::endl << key_pair.second.encoded() << std::endl;
    std::cout << "EC public key: " << std::endl << key_pair.first.encoded() << std::endl;
    return 0;
}
