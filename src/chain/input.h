#ifndef LIBBITCOIN_THESIS_INPUT_H
#define LIBBITCOIN_THESIS_INPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class input {
public:
    input();

    input(const bc::chain::output_point& previous_output);

    input(const bc::hash_digest& hash, uint32_t index);

    void set_previous_output(const bc::chain::output_point& previous_output);

    void set_sequence(uint32_t sequence);

private:
    bc::chain::output_point _previous_output;
    bc::data_chunk _script_sig;
    uint32_t _sequence;
};


#endif //LIBBITCOIN_THESIS_INPUT_H
