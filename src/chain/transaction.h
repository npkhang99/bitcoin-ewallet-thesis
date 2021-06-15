#ifndef LIBBITCOIN_THESIS_TRANSACTION_H
#define LIBBITCOIN_THESIS_TRANSACTION_H

#include <cstdint>
#include <bitcoin/bitcoin.hpp>

#include "../hd_wallet/payment_address.h"

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

private:
    uint32_t _version;
    uint32_t _locktime;
    std::vector<input> _inputs;
    std::vector<output> _outputs;
};


#endif //LIBBITCOIN_THESIS_TRANSACTION_H
