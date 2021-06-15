#ifndef LIBBITCOIN_THESIS_CHAIN_UTILITIES_H
#define LIBBITCOIN_THESIS_CHAIN_UTILITIES_H

#include <bitcoin/bitcoin.hpp>

#include "transaction.h"

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
 * Sign all inputs of a given transaction
 * @param tx the transaction to be signed
 * @param pubkey the public key
 * @param pubkey_hash the public key HASH160 digest
 * @param secret the secret key
 * @param sighash_type signature hash type
 */
void sign(transaction& tx,
          const bc::byte_array<bc::ec_compressed_size>& pubkey,
          const bc::short_hash& pubkey_hash,
          const bc::byte_array<bc::ec_secret_size>& secret,
          uint8_t sighash_type);

#endif //LIBBITCOIN_THESIS_CHAIN_UTILITIES_H
