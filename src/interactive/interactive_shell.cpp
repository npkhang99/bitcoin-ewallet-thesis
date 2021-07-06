#include "interactive_shell.h"

interactive_shell::interactive_shell() {
    wallet = nullptr;
    testnet = false;
    APP_LOOP = true;
}

int interactive_shell::run(int argc, const char* argv[]) {
    if (argc > 2) {
        std::cout << format(usage, argv[0]) << std::endl;
        return 1;
    }

    if (argc == 2 && strcmp(argv[1], "mainnet") != 0 &&
            strcmp(argv[1], "testnet") != 0) {
        std::cout << format(usage, argv[0]) << std::endl;
        return 1;
    }

    if (argc == 2 && !strcmp(argv[1], "testnet")) {
        testnet = true;
    }

    while (APP_LOOP) {
        std::string command;
        std::cout << "> ";
        std::cout.flush();
        std::cin >> command;

        std::cout << command << std::endl;

        switch (get_command(command, wallet)) {
            case commands::NEW_WALLET:
                init_wallet();
                break;
            case commands::LOAD_WALLET:
                load_wallet();
                break;
            case commands::GET_BALANCE:
                std::cout << "Your wallet balance:" << std::endl;
                wallet->get_balance();
                break;
            case commands::NEW_ADDRESS:
                std::cout << "Your payment address: "
                          << wallet->get_new_payment_address().encoded()
                          << std::endl;
                break;
            case commands::LIST_TRANSACTIONS:
                throw std::runtime_error("Un-implemented error");
            case commands::NEW_TRANSACTION: {
                transaction_builder tbuilder(wallet, testnet);
                transaction tx;
                if (tbuilder.build(tx)) {
                    std::cout << "Your raw transaction: "
                              << bc::encode_base16(tx.to_data()) << std::endl;
                } else {
                    std::cout << "Transaction building canceled" << std::endl;
                }
                break;
            }
            case commands::EXIT:
                cin_clear_line();
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

bool interactive_shell::init_wallet(const std::vector<std::string>& mnemonic,
                                    const std::string& passphrase) {
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

void interactive_shell::load_wallet() {
    std::vector<std::string> mnemonic;
    std::string line;
    std::getline(std::cin, line);
    std::stringstream ss(line);

    std::string token;
    while (ss >> token) {
        mnemonic.emplace_back(token);
    }

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
