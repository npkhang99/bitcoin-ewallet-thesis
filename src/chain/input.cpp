#include "input.h"

input::input() {}

const std::string& input::get_txid() const {
    return _txid;
}

void input::set_txid(const std::string& txid) {
    _txid = txid;
}

uint8_t input::get_vout() const {
    return vout;
}

void input::set_vout(uint8_t vout) {
    input::vout = vout;
}

uint32_t input::get_sequence() const {
    return sequence;
}

void input::set_sequence(uint32_t sequence) {
    input::sequence = sequence;
}
