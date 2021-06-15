#ifndef LIBBITCOIN_THESIS_INPUT_H
#define LIBBITCOIN_THESIS_INPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class input {
public:
    input();

    input(const input& o);

    input(const bc::chain::output_point& previous_output);

    input(const bc::hash_digest& hash, uint32_t index);

    input(const bc::chain::output_point& previous_output,
          const bc::chain::script& script, uint32_t sequence);

    void set_previous_output(const bc::chain::output_point& previous_output);

    void set_previous_output(const bc::hash_digest& hash, uint32_t index);

    void set_script_sig(const bc::chain::script& script_sig);

    void set_sequence(uint32_t sequence);

    const bc::chain::output_point& get_previous_output() const;

    const bc::chain::script& get_script() const;

    uint32_t get_sequence() const;

    bc::data_chunk to_data() const;

private:
    bc::chain::output_point _previous_output;
    bc::chain::script _script;
    uint32_t _sequence;
};


#endif //LIBBITCOIN_THESIS_INPUT_H
