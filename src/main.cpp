#include <cstring>

#include "interactive/interactive_shell.h"

#ifdef DEBUG
#include "tests.h"
#endif

#ifdef DEBUG
void run_tests() {
    test_address();
    test_hd_wallet();
    test_hd_wallet_keys();
    test_bip32();
    test_transaction();
    test_transaction_testnet();
    test_get_balance();
    test_get_fee();
}
#endif

int main(int argc, const char* argv[]) {
#ifdef DEBUG
    if (argc == 2 && !std::strcmp(argv[1], "test")) {
        run_tests();
        return 0;
    }
#endif

    interactive_shell shell;
    return shell.run(argc, argv);
}
