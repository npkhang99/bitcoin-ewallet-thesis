#include "tests.h"

int main() {
    test_address();
    test_hd_wallet();
    test_hd_wallet_keys();
    test_bip32();
    test_transaction();
    test_transaction_testnet();
    return 0;
}
