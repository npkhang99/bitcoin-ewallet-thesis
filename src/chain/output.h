#ifndef LIBBITCOIN_THESIS_OUTPUT_H
#define LIBBITCOIN_THESIS_OUTPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class output {
public:
    output();

    output(uint64_t satoshi, const bc::chain::script& script);

    uint64_t get_satoshi() const;

    void set_satoshi(uint64_t satoshi);

    const bc::chain::script& get_script_pub_key() const;

    void set_script(const bc::chain::script& script_pub_key);

    bc::data_chunk to_data() const;

private:
    uint64_t _satoshi;
    bc::chain::script _script;
};


#endif //LIBBITCOIN_THESIS_OUTPUT_H
