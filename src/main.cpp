#include <iostream>
#include <string>

#include "wallet/hd_wallet.h"

#include "data.h"
#include "utilities.h"

#ifdef DEBUG
#include "tests.h"
#endif

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

bool init_wallet(const std::vector<std::string>& mnemonic = {},
                 const std::string& passphrase = "") {
    if (wallet != nullptr) {
        delete wallet;
        wallet = nullptr;
    }

    if (mnemonic.empty()) {
        wallet = new hd_wallet(testnet);
        std::cout << "Passphrase (optional): ";

        std::string p;
        std::getline(std::cin, p);

        wallet->set_passphrase(p);
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

    std::cout << "Exploring wallet... ";
    std::cout.flush();

    wallet->explore();

    std::cout << "success" << std::endl;

    return true;
}

void load_wallet() {
    std::vector<std::string> mnemonic;
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    std::string token;
    while (ss >> token) {
        mnemonic.emplace_back(token);
    }

    cin_clear_line();
    std::cout << "Passphrase (if your wallet have one): ";
    std::string passphrase;
    std::getline(std::cin, passphrase);

    if (!init_wallet(mnemonic, passphrase)) {
        std::cerr << "wallet initialization failed" << std::endl;
    } else {
        std::cout << "Wallet initialized successfully" << std::endl;
        std::cout << "Your wallet balance:" << std::endl;
        wallet->get_balance();
    }
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
            case commands::LOAD_WALLET:
                load_wallet();
                break;
            }
            case commands::EXIT:
                APP_LOOP = false;
                break;
            case commands::HELP:
            default:
                cin_clear_line();
                print_help();
        }
    }

    if (wallet != nullptr) {
        delete wallet;
        wallet = nullptr;
    }

    return 0;
}
