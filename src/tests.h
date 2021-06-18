#ifndef LIBBITCOIN_THESIS_TESTS_H
#define LIBBITCOIN_THESIS_TESTS_H

#include <iostream>
#include <string>
#include <bitcoin/bitcoin.hpp>

#include "wallet/hd_private.h"
#include "wallet/hd_public.h"
#include "wallet/hd_wallet.h"
#include "wallet/payment_address.h"

#include "chain/transaction.h"
#include "chain/chain_utilities.h"

#include "utilities.h"

void test_address();

void test_hd_wallet();

void test_hd_wallet_keys();

void test_bip32();

void test_transaction();

void test_transaction_testnet();

void test_get_balance();

#endif //LIBBITCOIN_THESIS_TESTS_H
