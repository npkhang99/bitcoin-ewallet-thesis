#include "output.h"

output::output() : _satoshi(0) {}

output::output(uint64_t satoshi, const bc::chain::script& script)
        : _satoshi(satoshi), _script(script) {}

uint64_t output::get_satoshi() const {
    return _satoshi;
}

void output::set_satoshi(uint64_t satoshi) {
    _satoshi = satoshi;
}

const bc::chain::script& output::get_script_pub_key() const {
    return _script;
}

void output::set_script(const bc::chain::script& script_pub_key) {
    _script = script_pub_key;
}

bc::data_chunk output::to_data() const {
    bc::data_chunk raw;
    bc::extend_data(raw, bc::to_little_endian(_satoshi));
    bc::extend_data(raw, _script.to_data(true));
    return raw;
}
