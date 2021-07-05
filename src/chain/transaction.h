#ifndef LIBBITCOIN_THESIS_TRANSACTION_H
#define LIBBITCOIN_THESIS_TRANSACTION_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

#include "../wallet/payment_address.h"
#include "../client/client_utilities.h"

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
    void set_fee(uint64_t fee);

    void set_network(const std::string& network);

    uint64_t get_total_fund() const;
    uint64_t get_total_spends() const;

    void add_change_output(const payment_address& address);

    uint32_t get_version() const;
    uint32_t get_locktime() const;

    bc::data_chunk to_data() const;

    void sign(uint32_t index,
              const bc::byte_array<bc::ec_compressed_size>& pubkey,
              const bc::short_hash& pubkey_hash,
              const bc::byte_array<bc::ec_secret_size>& secret,
              uint8_t sighash_type);

    uint64_t get_max_fee() const;

private:
    uint32_t _version;
    uint32_t _locktime;
    std::vector<input> _inputs;
    std::vector<output> _outputs;

    uint64_t _fee;
    std::string _network = "mainnet";

    void prepare_inputs(std::vector<input>& inputs, uint32_t index,
                        const bc::chain::script& original_script_code);

    // sign and return DER encoded EC signature
    bc::data_chunk sign(const transaction& tx,
                        const bc::chain::script& script_code,
                        const bc::byte_array<bc::ec_secret_size>& secret,
                        uint8_t sighash_type);
};


#endif //LIBBITCOIN_THESIS_TRANSACTION_H
