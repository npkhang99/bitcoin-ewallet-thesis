#ifndef LIBBITCOIN_THESIS_TESTS_H
#define LIBBITCOIN_THESIS_TESTS_H

#include <iostream>
#include <string>
#include <bitcoin/bitcoin.hpp>

#include "hd_private.h"
#include "hd_public.h"
#include "hd_wallet.h"
#include "payment_address.h"

#include "utilities.h"

void test_address();

void test_hd_wallet();

void test_hd_wallet_keys();

void test_bip32();

#endif //LIBBITCOIN_THESIS_TESTS_H
