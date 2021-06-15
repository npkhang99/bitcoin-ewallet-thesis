#include "input.h"

input::input() {}

input::input(const input& o) :
        _previous_output(o._previous_output), _script(o._script),
        _sequence(o._sequence) {}

input::input(const bc::chain::output_point& previous_output) :
        _previous_output(previous_output) {}

input::input(const bc::hash_digest& hash, uint32_t index) :
        _previous_output(hash, index) {}

input::input(const bc::chain::output_point& previous_output,
             const bc::chain::script& script, uint32_t sequence)
        : _previous_output(previous_output), _script(script),
          _sequence(sequence) {}

void input::set_previous_output(const bc::chain::output_point& previous_output) {
    _previous_output = previous_output;
}

void input::set_previous_output(const bc::hash_digest& hash, uint32_t index) {
    _previous_output = bc::chain::output_point(hash, index);
}

void input::set_script_sig(const bc::chain::script& script_sig) {
    _script = script_sig;
}

void input::set_sequence(uint32_t sequence) {
    _sequence = sequence;
}

const bc::chain::output_point& input::get_previous_output() const {
    return _previous_output;
}

const bc::chain::script& input::get_script() const {
    return _script;
}

uint32_t input::get_sequence() const {
    return _sequence;
}

bc::data_chunk input::to_data() const {
    bc::data_chunk raw;
    bc::extend_data(raw, _previous_output.to_data());
    bc::extend_data(raw, _script.to_data(true));
    bc::extend_data(raw, bc::to_little_endian(_sequence));
    return raw;
}
