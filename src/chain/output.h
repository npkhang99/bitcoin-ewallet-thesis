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

    /**
     * Manually create p2kh locking script with corresponding public key point
     * hash
     * @param pubkey the public key point hash
     * @return list of p2kh operations
     */
    static bc::machine::operation::list make_pay_key_hash(const bc::short_hash& pubkey);

private:
    uint64_t _satoshi;
    bc::data_slice _script_pub_key;
};


#endif //LIBBITCOIN_THESIS_OUTPUT_H
