#ifndef LIBBITCOIN_THESIS_OUTPUT_H
#define LIBBITCOIN_THESIS_OUTPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class output {
public:
    uint64_t get_satoshi() const;

    void set_satoshi(uint64_t satoshi);

    const bc::data_slice& get_script_pub_key() const;

    void set_script_pub_key(const bc::data_slice& script_pub_key);

    static bc::data_chunk to_pay_key_hash(const bc::short_hash& hash);

private:
    uint64_t _satoshi;
    bc::data_slice _script_pub_key;
};


#endif //LIBBITCOIN_THESIS_OUTPUT_H
