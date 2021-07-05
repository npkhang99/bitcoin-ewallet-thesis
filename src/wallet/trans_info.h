#ifndef LIBBITCOIN_THESIS_TRANS_INFO_H
#define LIBBITCOIN_THESIS_TRANS_INFO_H

#include <bitcoin/bitcoin.hpp>

struct trans_info {
    hd_private priv;
    bc::chain::output_point previous_tx;
    uint64_t value;
};

#endif //LIBBITCOIN_THESIS_TRANS_INFO_H
