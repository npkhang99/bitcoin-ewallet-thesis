#include "input.h"

input::input() {}

input::input(const bc::chain::output_point& previous_output) :
        _previous_output(previous_output) {}

input::input(const bc::hash_digest& hash, uint32_t index) :
        _previous_output(hash, index) {}

void input::set_previous_output(const bc::chain::output_point& previous_output) {
    _previous_output = previous_output;
}

void input::set_sequence(uint32_t sequence) {
    _sequence = sequence;
}
