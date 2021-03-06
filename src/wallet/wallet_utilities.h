#ifndef LIBBITCOIN_THESIS_WALLET_UTILITIES_H
#define LIBBITCOIN_THESIS_WALLET_UTILITIES_H

#include <iostream>
#include <string>
#include <functional>

#include <bitcoin/bitcoin.hpp>
#include <secp256k1.h>

#include "../client/client_utilities.h"

typedef bc::byte_array<33> ec_public;
typedef bc::data_chunk ec_private;

typedef std::pair<ec_public, ec_private> ec_key_pair;

/**
 * Generate an EC Secret for the corresponding secret hex
 * If secret hex is not given, generate a new secret
 * @param secret_hex the EC Secret hex to generate from
 * @return a secret key
 */
bc::ec_secret generate_secret(const std::string& secret_hex);

/**
 * Generate a pair of wallet public and private key from given EC secret
 * EC private key generated will be compressed, same for EC public key
 * @param secret the EC secret
 * @return a pair of wallet public and private key correspondingly
 */
ec_key_pair generate_wallet_ec_key_pair(const bc::ec_secret& secret);

/**
 * Generate a compressed public key from given ec_secret
 * In other words, return the coordinate pair resulting from EC point
 * multiplication of the secp256k1 base point with the ec_secret
 * @param secret the ec_secret to create public key from
 * @return data_chunk of public key
 */
bc::byte_array<33> secp256k1_point(const bc::ec_secret& secret);

/**
 * Get current price for given satoshi
 * @param satoshi satoshi to convert
 * @return string representation of the pricing
 */
std::string get_price(uint64_t satoshi);

#endif //LIBBITCOIN_THESIS_WALLET_UTILITIES_H
