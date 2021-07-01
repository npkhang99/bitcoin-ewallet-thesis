#ifndef LIBBITCOIN_THESIS_CHAIN_UTILITIES_H
#define LIBBITCOIN_THESIS_CHAIN_UTILITIES_H

#include <bitcoin/bitcoin.hpp>
#include <bitcoin/client.hpp>

#include <curl/curl.h>
#include <json/json.h>
#include <sstream>

#include "transaction.h"
#include "../client/http_client.h"

const std::string mainnet = "BTC";
const std::string testnet = "BTCTEST";

/**
 * Create p2pkh locking script (script pub key)
 * @param pubkey_hash the public key point hash
 * @return p2pkh locking script
 */
bc::chain::script make_locking_script(const bc::short_hash& pubkey_hash);

/**
 * Create p2pkh unlocking script (script sig)
 * @param pubkey the public key point hash
 * @param signature the DER encoded EC signature
 * @return p2pkh unlocking script
 */
bc::chain::script make_unlocking_script(
        const bc::byte_array<bc::ec_compressed_size>& pubkey,
        const bc::data_chunk& signature);

/**
 * Create a EC signature hash
 * @param tx the transaction
 * @param sighash_type the hash type
 * @return hash digest
 */
bc::hash_digest create_signature_hash(const transaction& tx,
                                      uint32_t sighash_type);

/**
 * Strip the script codes separators for signing transactions because
 * bitcoin-core will freak out when sign input with separators in the op codes
 * @param script_code the original script
 * @return the original script with separators stripped
 */
bc::chain::script strip_code_separators(const bc::chain::script& script_code);

/**
 * Get balance of a given bitcoin address
 * @param address the encoded address
 * @param url the network url to connect
 * @return true if success, otherwise false
 */
bool get_balance(std::string& out, const std::string& address,
                 const std::string& network = mainnet);

/**
 * Get fee for fastest transaction confirmation
 * @return fee per transaction byte in satoshi
 */
uint64_t get_recommended_fee();

#endif //LIBBITCOIN_THESIS_CHAIN_UTILITIES_H
