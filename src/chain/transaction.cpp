#include "transaction.h"

#include "chain_utilities.h"

transaction::transaction()
        : _version(0), _locktime(0) {}

transaction::transaction(uint32_t version, uint32_t locktime)
        : _version(version), _locktime(locktime) {}

transaction::transaction(uint32_t version, uint32_t locktime,
                         const std::vector<input>& inputs,
                         const std::vector<output>& outputs)
        : _version(version), _locktime(locktime), _inputs(inputs),
            _outputs(outputs) {}

void transaction::set_version(uint32_t version) {
    _version = version;
}

void transaction::set_locktime(uint32_t locktime) {
    _locktime = locktime;
}

void transaction::set_inputs(const std::vector<input>& inputs) {
    _inputs = inputs;
}

void transaction::set_outputs(const std::vector<output>& outputs) {
    _outputs = outputs;
}

uint32_t transaction::get_version() const {
    return _version;
}

uint32_t transaction::get_locktime() const {
    return _locktime;
}

void transaction::add_input(const input& in) {
    _inputs.push_back(in);
}

void transaction::add_output(const output& out) {
    _outputs.push_back(out);
}

void transaction::set_message(const std::string& message) {
    bc::machine::operation op_message =
            bc::machine::operation(bc::to_chunk(message));

    bc::machine::operation::list message_ops;
    message_ops.push_back(bc::machine::opcode::return_);
    message_ops.push_back(op_message);

    bc::chain::script message_script = bc::chain::script(message_ops);

    output message_out;
    message_out.set_satoshi(0);
    message_out.set_script(message_script);

    _outputs.push_back(message_out);
}

void transaction::set_fee(uint64_t fee) {
    _fee = fee;
}

void transaction::set_network(const std::string& network) {
    _network = network;
}

void transaction::add_change_output(const payment_address& address) {
    if (_fee == 0) {
        _fee = get_recommended_fee();
    }

    uint64_t total_spend = get_spend_satoshi();

    uint64_t total_used = 0;
    for (const output& out : _outputs) {
        total_used += out.get_satoshi();
    }

    output change;
    change.set_satoshi(0);
    change.set_script(make_locking_script(address.get_hash()));
    _outputs.push_back(change);

    uint64_t bytes = to_data().size();
    change.set_satoshi(total_spend - total_used - _fee * bytes);

    _outputs.pop_back();
    _outputs.push_back(change);
}

template <class T>
void serialize_vector(bc::data_chunk& raw, const std::vector<T>& v) {
    raw.push_back(v.size());
    for (const T& e : v) {
        bc::extend_data(raw, e.to_data());
    }
}

bc::data_chunk transaction::to_data() const {
    bc::data_chunk raw;

    bc::extend_data(raw, bc::to_little_endian(_version));
    serialize_vector(raw, _inputs);
    serialize_vector(raw, _outputs);
    bc::extend_data(raw, bc::to_little_endian(_locktime));

    return raw;
}

void transaction::prepare_inputs(std::vector<input>& inputs, uint32_t index,
                    const bc::chain::script& original_script_code) {
    // erase all input scripts
    for (const auto& in : _inputs) {
        inputs.push_back(input(in.get_previous_output(), bc::chain::script{},
                               in.get_sequence()));
    }

    // restore the script of the input we want to sign
    inputs[index].set_script_sig(original_script_code);
}

bc::data_chunk transaction::sign(const transaction& tx,
                                 const bc::chain::script& script_code,
                                 const bc::byte_array<bc::ec_secret_size>& secret,
                                 uint8_t sighash_type) {
    bc::hash_digest sighash = create_signature_hash(tx, sighash_type);
    // EC signature
    bc::ec_signature signature;
    if (!bc::sign(signature, secret, sighash)) {
        throw std::runtime_error("something went wrong when signing transaction");
    }

    // encode EC signature as DER
    bc::data_chunk der;
    if (!bc::encode_signature(der, signature)) {
        throw std::runtime_error("something went wrong when encode signature");
    }

    // add sighash type to end of DER sequence
    der.push_back(sighash_type);
    return der;
}

void transaction::sign(uint32_t index,
                       const bc::byte_array<bc::ec_compressed_size>& pubkey,
                       const bc::short_hash& pubkey_hash,
                       const bc::byte_array<bc::ec_secret_size>& secret,
                       uint8_t sighash_type) {
    const auto script_code = make_locking_script(pubkey_hash);
    const auto stripped = strip_code_separators(script_code);

    std::vector<input> inputs;
    prepare_inputs(inputs, index, stripped);

    transaction sign_tx(_version, _locktime, inputs, _outputs);

    const auto signature = sign(sign_tx, stripped, secret, sighash_type);
    const auto script_sig = make_unlocking_script(pubkey, signature);

    _inputs[index].set_script_sig(script_sig);
}

uint64_t transaction::get_spend_satoshi() {
    uint64_t total_spent = 0;

    for (const input& in : _inputs) {
        std::string tx_hash = bc::encode_hash(in.get_previous_output().hash());
        uint32_t index = in.get_previous_output().index();

        Json::Value tx_info;
        client::get_tx_info(tx_info, tx_hash, _network == "testnet" ?
                                client::testnet : client::mainnet);

        uint64_t value;
        bc::decode_base10(value, tx_info["outputs"][index]["value"].asString(), 8);

        total_spent += value;
    }

    return total_spent;
}
