#include <iostream>
#include <string>

#include "functions.h"

int main() {
    bc::ec_secret secret_ec = generate_secret(
            "067375cf2f49e9bc7b759acdcc6b9cd43326a550415eff5ba8d5e266d7adfca3");
    std::cout << "EC secret:" << std::endl << bc::encode_base16(secret_ec)
              << std::endl;

    auto key_pair = generate_wallet_ec_key_pair(secret_ec);
    std::cout << "EC private key:" << std::endl << key_pair.second.encoded()
              << std::endl;
    std::cout << "EC public key:" << std::endl << key_pair.first.encoded()
              << std::endl;

    std::string address = generate_address(key_pair);
    std::cout << "Bitcoin address:" << std::endl << address << std::endl;
    return 0;
}
