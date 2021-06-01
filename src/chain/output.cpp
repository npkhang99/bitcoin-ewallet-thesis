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

bc::data_chunk output::to_pay_key_hash(const bc::short_hash& hash) {
    return bc::build_chunk({
        bc::to_chunk(bc::machine::opcode::dup),
        bc::to_chunk(bc::machine::opcode::hash160),
        hash,
        bc::to_chunk(bc::machine::opcode::equalverify),
        bc::to_chunk(bc::machine::opcode::checksig)
    });
}
