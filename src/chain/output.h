#ifndef LIBBITCOIN_THESIS_OUTPUT_H
#define LIBBITCOIN_THESIS_OUTPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class output {
public:
    uint64_t get_satoshi() const;

    void set_satoshi(uint64_t satoshi);

    const bc::data_chunk& get_script_pub_key() const;

    void set_script_pub_key(const bc::data_chunk& script_pub_key);

    /**
     * Create p2kh locking script (script pub key) with corresponding public key
     * point hash
     * @param pubkey the public key point hash
     * @return list of p2kh operations
     */
    bc::data_chunk make_locking_script(const bc::short_hash& pubkey);

    bc::data_chunk to_data() const;

private:
    uint64_t _satoshi;
    bc::data_chunk _script_pub_key;
};


#endif //LIBBITCOIN_THESIS_OUTPUT_H
