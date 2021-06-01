#ifndef LIBBITCOIN_THESIS_INPUT_H
#define LIBBITCOIN_THESIS_INPUT_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

class input {
public:
    input();

    const std::string& get_txid() const;

    void set_txid(const std::string& txid);

    uint8_t get_vout() const;

    void set_vout(uint8_t vout);

    uint32_t get_sequence() const;

    void set_sequence(uint32_t sequence);

private:
    std::string _txid;
    uint8_t vout;
    bc::data_chunk _script_sig;
    uint32_t sequence;
};


#endif //LIBBITCOIN_THESIS_INPUT_H
