#ifndef LIBBITCOIN_THESIS_UTILITIES_H
#define LIBBITCOIN_THESIS_UTILITIES_H

#include <string>

#include "wallet/hd_wallet.h"

#include "data.h"

std::string format(const char* fmt, ...);

commands get_command(const std::string& arg, const hd_wallet* wallet);

#endif //LIBBITCOIN_THESIS_UTILITIES_H
