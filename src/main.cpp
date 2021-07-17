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

    bool testnet = false;
    bool daemon = false;

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-d")) {
            daemon = true;
        } else if (!strcmp(argv[i], "mainnet")) {
            testnet = false;
        } else if (!strcmp(argv[i], "testnet")) {
            testnet = true;
        } else if (!strcmp(argv[i], "help")) {
            std::cout << format(usage, argv[0]) << std::endl;
            return 1;
        } else {
            std::cout << "Unrecognized option(s)" << std::endl;
            std::cout << format(usage, argv[0]) << std::endl;
            return 1;
        }
    }

    interactive_shell shell(testnet);
    return shell.run();
}
