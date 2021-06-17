#ifndef LIBBITCOIN_THESIS_TRANSACTION_H
#define LIBBITCOIN_THESIS_TRANSACTION_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

#include "../wallet/payment_address.h"

#include "input.h"
#include "output.h"

class transaction {
public:
    transaction();
    transaction(uint32_t version, uint32_t locktime);
    transaction(uint32_t version, uint32_t locktime,
                const std::vector<input>& inputs,
                const std::vector<output>& outputs);

    void set_version(uint32_t version);
    void set_locktime(uint32_t locktime);
    void set_inputs(const std::vector<input>& inputs);
    void set_outputs(const std::vector<output>& outputs);

    void add_input(const input& in);
    void add_output(const output& out);

    void set_message(const std::string& message);

    uint32_t get_version() const;
    uint32_t get_locktime() const;

    // accessors
    std::vector<input>& inputs();
    std::vector<output>& outputs();

    bc::data_chunk to_data() const;

    // sign and return DER encoded EC signature
    bc::data_chunk sign_input(uint32_t index, const bc::chain::script& script_code,
                              const bc::byte_array<bc::ec_secret_size>& secret,
                              uint8_t sighash_type);

private:
    uint32_t _version;
    uint32_t _locktime;
    std::vector<input> _inputs;
    std::vector<output> _outputs;
};


#endif //LIBBITCOIN_THESIS_TRANSACTION_H
