#ifndef LIBBITCOIN_THESIS_HD_LINEAGE_H
#define LIBBITCOIN_THESIS_HD_LINEAGE_H

#include <iostream>

struct hd_lineage {
    uint32_t version;
    uint8_t depth;
    uint32_t fingerprint;
    uint32_t index;
};

#endif //LIBBITCOIN_THESIS_HD_LINEAGE_H
