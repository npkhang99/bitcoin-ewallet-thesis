#include "output.h"

uint64_t output::get_satoshi() const {
    return _satoshi;
}

void output::set_satoshi(uint64_t satoshi) {
    _satoshi = satoshi;
}

const bc::data_chunk& output::get_script_pub_key() const {
    return _script_pub_key;
}

void output::set_script_pub_key(const bc::data_chunk& script_pub_key) {
    _script_pub_key = script_pub_key;
}

bc::data_chunk output::make_locking_script(const bc::short_hash& pubkey) {
    bc::machine::operation::list p2kh;
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::dup));
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::hash160));

    bc::machine::operation op_pubkey = bc::machine::operation(bc::to_chunk(pubkey));
    p2kh.push_back(op_pubkey);

    p2kh.push_back(bc::machine::operation(bc::machine::opcode::equalverify));
    p2kh.push_back(bc::machine::operation(bc::machine::opcode::checksig));

#ifdef DEBUG
    bc::machine::operation::list control_p2kh =
            bc::chain::script::to_pay_key_hash_pattern(pubkey);
    assert(control_p2kh == p2kh);
#endif

    return bc::chain::script(p2kh).to_data(true);
}

bc::data_chunk output::to_data() const {
    bc::data_chunk output_chunk;
    bc::extend_data(output_chunk, bc::to_little_endian(_satoshi));
    output_chunk.push_back(_script_pub_key.size());
    bc::extend_data(output_chunk, _script_pub_key);
    return bc::data_chunk();
}
