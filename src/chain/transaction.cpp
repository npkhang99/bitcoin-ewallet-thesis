#include "transaction.h"

transaction::transaction()
    : _version(0), _locktime(0) {}

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
