#include <iostream>
#include <string>

#include "wallet/hd_wallet.h"

#include "data.h"
#include "utilities.h"

#ifdef DEBUG
#include "tests.h"
#endif

const char* usage = "usage: %s [chain]\nOptions:\n"
                    "  chain: 'mainnet' (default) or 'testnet'";

const char* help[] = {
        "exit: exit the program",
        "help: print this usage message",
        "newwallet: create a new HD wallet",
        "loadwallet <mnemonic>: initialize a new HD wallet from <mnemonic>",
        "getbalance: get current wallet balance",
        "listtransactions: list all transactions made using this wallet"
};

bool APP_LOOP = true;

hd_wallet* wallet = nullptr;
bool testnet = true;

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

bool init_wallet(const std::string& mnemonic = "",
                 const std::string& passphrase = "") {
    if (mnemonic.empty()) {
        wallet = new hd_wallet();
        return true;
    }

    try {
        wallet = new hd_wallet(mnemonic, testnet);
    } catch (std::exception& e) {
        std::cerr << "error while create new exception "
                  << e.what() << std::endl;
        delete wallet;
        wallet = nullptr;
        return false;
    }

    if (!passphrase.empty()) {
        wallet->set_passphrase(passphrase);
    }

    return true;
}

int main(int argv, const char* args[]) {
    if (argv > 2) {
        std::cout << format(usage, args[0]) << std::endl;
        return 1;
    }

#ifdef DEBUG
    if (strcmp(args[1], "test") == 0) {
        run_tests();
        return 0;
    }
#endif

    if (strcmp(args[1], "mainnet") != 0 && strcmp(args[1], "testnet") != 0) {
        std::cout << format(usage, args[0]) << std::endl;
        return 1;
    }

    while (APP_LOOP) {
        std::string command;
        std::cout << "> ";
        std::cin >> command;

        std::cout << command << std::endl;

        switch (get_command(command, wallet)) {
            case commands::NEW_WALLET:
                init_wallet();
                break;
            case commands::LOAD_WALLET: {
                std::string mnemonic;
                std::cin.ignore();
                std::getline(std::cin, mnemonic);

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                std::cout << "Passphrase (if your wallet have one): ";
                std::string passphrase;
                std::getline(std::cin, passphrase);

                init_wallet(mnemonic, passphrase);
                break;
            }
            case commands::EXIT:
                APP_LOOP = false;
                break;
            case commands::HELP:
            default:
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
                                '\n');
                std::cout << "Support commands:" << std::endl;
                for (const auto& i : help) {
                    std::cout << i << std::endl;
                }
        }
    }

    if (wallet != nullptr) {
        delete wallet;
        wallet = nullptr;
    }

    return 0;
}
