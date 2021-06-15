#include "chain_utilities.h"

bc::chain::script make_locking_script(const bc::short_hash& pubkey_hash) {
    bc::machine::operation::list p2kh;
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::dup));
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::hash160));

    bc::machine::operation op_pubkey_hash =
            bc::machine::operation(bc::to_chunk(pubkey_hash));
    p2kh.push_back(op_pubkey_hash);

    p2kh.push_back(bc::machine::operation(bc::machine::opcode::equalverify));
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::checksig));

    return bc::chain::script(p2kh);
}

bc::chain::script make_unlocking_script(
        const bc::byte_array<bc::ec_compressed_size>& pubkey,
        const bc::data_chunk& signature) {
    bc::machine::operation::list script_sig;
    script_sig.push_back(bc::machine::operation(signature));
    script_sig.push_back(bc::machine::operation(bc::to_chunk(pubkey)));
    return bc::chain::script(script_sig);
}

bc::chain::script strip_code_separators(const bc::chain::script& script_code) {
    bc::machine::operation::list ops;

    for (const auto& op : script_code.operations()) {
        if (op.code() != bc::machine::opcode::codeseparator) {
            ops.push_back(op);
        }
    }

    return bc::chain::script(ops);
}

bc::hash_digest create_signature_hash(const transaction& tx,
                                      uint32_t sighash_type) {
    auto serialized = tx.to_data();
    bc::extend_data(serialized, bc::to_little_endian(sighash_type));
    return bc::bitcoin_hash(serialized);
}

void sign(transaction& tx, const bc::byte_array<bc::ec_compressed_size>& pubkey,
          const bc::short_hash& pubkey_hash,
          const bc::byte_array<bc::ec_secret_size>& secret,
          uint8_t sighash_type) {
    const auto script_code = make_locking_script(pubkey_hash);
    const auto stripped = strip_code_separators(script_code);

    for (uint32_t i = 0; i < tx.inputs().size(); i++) {
        const auto signature = tx.sign_input(i, stripped, secret, sighash_type);
        const auto script_sig = make_unlocking_script(pubkey, signature);
        tx.inputs()[i].set_script_sig(script_sig);
    }
}
