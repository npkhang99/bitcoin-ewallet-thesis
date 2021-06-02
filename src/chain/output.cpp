#include "output.h"

uint64_t output::get_satoshi() const {
    return _satoshi;
}

void output::set_satoshi(uint64_t satoshi) {
    _satoshi = satoshi;
}

const bc::data_slice& output::get_script_pub_key() const {
    return _script_pub_key;
}

void output::set_script_pub_key(const bc::data_slice& script_pub_key) {
    _script_pub_key = script_pub_key;
}

bc::machine::operation::list output::make_pay_key_hash(const bc::short_hash& pubkey) {
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

    return p2kh;
}
