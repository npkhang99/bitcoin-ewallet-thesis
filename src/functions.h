#ifndef LIBBITCOIN_THESIS_FUNCTIONS_H
#define LIBBITCOIN_THESIS_FUNCTIONS_H

#include <string>
#include <functional>

#include <bitcoin/bitcoin.hpp>

typedef std::pair<bc::wallet::ec_public, bc::wallet::ec_private> ec_key_pair;

/**
 * Generate an EC Secret for the corresponding secret hex
 * If secret hex is not given, generate a new secret
 * @param secret_hex the EC Secret hex to generate from
 * @return a secret key
 */
bc::ec_secret generate_secret(const std::string &secret_hex);

/**
 * Generate a pair of wallet public and private key from given EC secret
 * @param secret the EC secret
 * @return a pair of wallet public and private key correspondingly
 */
ec_key_pair generate_wallet_ec_key_pair(bc::ec_secret secret);

/**
 * Convert public key hash to Base58Check encoding
 * @param payload the public key hash -- ripemd160(sha256(public_key))
 * @return Base58Check encoding for the public key hash
 */
std::string base58_check_encode(const bc::short_hash &payload);

/**
 * Generate a bitcoin payment address for the corresponding EC key pair
 * @param key_pair the EC key pair
 * @return Bitcoin payment address
 */
std::string generate_address(const ec_key_pair &key_pair);

#endif //LIBBITCOIN_THESIS_FUNCTIONS_H
