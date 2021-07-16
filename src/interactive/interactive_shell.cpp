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

#ifdef DEBUG
        std::cout << command << std::endl;
#endif

        switch (get_command(command, wallet)) {
            case commands::NEW_WALLET:
                cin_clear_line();
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
            case commands::HISTORY:
                wallet->refresh();
                std::cout << "Your transaction history:" << std::endl;
                list_transaction_hist();
                break;
            case commands::NEW_TRANSACTION: {
                transaction_builder tbuilder(wallet, testnet);
                transaction tx;
                if (tbuilder.build(tx)) {
                    std::cout << "Your raw transaction: "
                              << bc::encode_base16(tx.to_data()) << std::endl;

                    std::cout << "Would you like to send this transaction to "
                                 "the network? (Y/n) ";

                    cin_clear_line();

                    bool send_tx = false;
                    std::string ans;
                    std::getline(std::cin, ans);

                    if (ans.length() == 0 || ans == "y" || ans == "Y") {
                        send_tx = true;
                    }

                    std::string txid;
                    if (send_tx && send_transaction(tx, txid)) {
                        std::cout << "Transaction sent successfully" << std::endl;
                        std::cout << "Your TXID: " << txid << std::endl;
                    }
                } else {
                    std::cout << "Transaction building canceled" << std::endl;
                }
                break;
            }
            case commands::REFRESH:
                std::cout << "Refreshing wallet... ";
                std::cout.flush();
                wallet->refresh();
                std::cout << "done" << std::endl;
                break;
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

bool interactive_shell::init_wallet(const std::vector<std::string>& mnemonic) {
    if (wallet != nullptr) {
        delete wallet;
        wallet = nullptr;
    }

    std::cout << "Passphrase (leave empty if none): ";
    std::string passphrase;
    std::getline(std::cin, passphrase);

    if (mnemonic.empty()) {
        wallet = new hd_wallet(testnet);
    } else {
        try {
            wallet = new hd_wallet(mnemonic, testnet);
        } catch (std::exception& e) {
            std::cout << "error while create new exception "
                    << e.what() << std::endl;
            delete wallet;
            wallet = nullptr;
            return false;
        }
    }

    if (!passphrase.empty()) {
        wallet->set_passphrase(passphrase);
        std::cout << "Passphrase applied" << std::endl;
    }

    if (!mnemonic.empty()) {
        std::cout << "Exploring wallet... ";
        std::cout.flush();

        wallet->explore();

        std::cout << "success" << std::endl;
    }

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

    if (!init_wallet(mnemonic)) {
        std::cout << "wallet initialization failed" << std::endl;
    } else {
        std::cout << "Wallet initialized successfully" << std::endl;
        std::cout << "Your wallet balance:" << std::endl;
        wallet->get_balance();
    }
}

void interactive_shell::list_transaction_hist() {
    for (const auto& address : wallet->get_used_payment_addresses()) {
        Json::Value info;
        client::get_address_info(info, address.encoded(),
                                 testnet ? client::testnet : client::mainnet);

        std::cout << "Address " << address.encoded() << ":" << std::endl;
        for (const auto& tx : info["txs"]) {
            time_t trans_time = tx["time"].asInt64();
            std::string btc = tx["incoming"]["value"].asString();

            uint64_t satoshi;
            bc::decode_base10(satoshi, btc, 8);

            std::string vnd = get_price(satoshi);

            std::cout << format("  TXID: %s", tx["txid"].asCString())
                      << std::endl;
            std::cout << format("    Block: %llu", tx["block_no"].asUInt64())
                      << std::endl;
            std::cout << format("    Confirmations: %u",tx["confirmations"].asUInt())
                      << std::endl;
            std::cout << format("    Value: %s BTC (%s VND)", btc.c_str(),
                                vnd.c_str()) << std::endl;
            std::cout << format("    Spent: %s", tx["incoming"]["spent"].isNull()
                                    ? "no" : tx["incoming"]["spent"].asCString())
                      << std::endl;
            std::cout << format("    Time: %s", asctime(localtime(&trans_time)))
                      << std::endl;
        }
    }
}

bool interactive_shell::send_transaction(const transaction& tx,
                                         std::string& txid) const {
    return client::send_raw_rx(txid, bc::encode_base16(tx.to_data()),
                               testnet ? client::testnet : client::mainnet);
}
