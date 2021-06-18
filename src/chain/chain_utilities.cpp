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

bc::hash_digest create_signature_hash(const transaction& tx,
                                      uint32_t sighash_type) {
    auto serialized = tx.to_data();
    bc::extend_data(serialized, bc::to_little_endian(sighash_type));
    return bc::bitcoin_hash(serialized);
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

uint64_t sum_transactions_value(const bc::chain::history::list& rows) {
    uint64_t unspent_balance = 0;
    for(const auto& row: rows) {
        // only count unspent transactions
        if (row.spend.hash() == bc::null_hash) {
            unspent_balance += row.value;
        }
    }

    return unspent_balance;
}

bool get_balance(std::string& out, const std::string& address,
                   const std::string& url) {
    bc::client::connection_type connection = {};
    connection.retries = 3;
    connection.timeout_seconds = 8;
    connection.server = bc::config::endpoint(url);

    bc::client::obelisk_client client(connection);

    bool success = false;

    // lambda function for history handler
    auto on_done = [&out](const bc::chain::history::list& rows) {
        uint64_t balance = sum_transactions_value(rows);
        out = bc::encode_base10(balance, 8);
    };

    // lambda function for error handler
    auto on_error = [&success](const bc::code& ec) {
        std::cerr << "An error occurred while fetching history!" << std::endl
                  << ec.message() << std::endl;
        success = false;
    };

    if (!client.connect(connection)) {
        std::cout << "Connection failed..." << std::endl;
        success = false;
    } else {
        std::cout << "Connection succeeded..." << std::endl;
        success = true;
    }

    client.blockchain_fetch_history3(on_error, on_done, {address});
    client.wait();

    return success;
}
