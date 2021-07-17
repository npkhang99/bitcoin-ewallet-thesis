#ifndef BITCOIN_EWALLET_THESIS_DATA_H
#define BITCOIN_EWALLET_THESIS_DATA_H

#include <iostream>
#include <string>

#include "../wallet/hd_wallet.h"

namespace server {
    static hd_wallet* wallet = nullptr;
    static std::string app_name = "SERVER";
}

#endif //BITCOIN_EWALLET_THESIS_DATA_H
