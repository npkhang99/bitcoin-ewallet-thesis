#include "transaction_builder.h"

transaction_builder::transaction_builder(hd_wallet* wallet, bool testnet) {
    if (wallet == nullptr) {
        throw std::invalid_argument("wallet must not be null");
    }

    this->wallet = wallet;
    this->testnet = testnet;
    unspents = this->wallet->get_unspent_txs();
}

bool verify_fee(uint64_t& fee, const std::string& fee_str) {
    for (char c : fee_str) {
        if (!std::isdigit(c)) {
            std::cout << "invalid fee" << std::endl;
            return false;
        }
    }

    try {
        fee = std::stoull(fee_str);
    } catch (std::exception& e) {
        std::cout << "invalid fee: " << e.what() << std::endl;
        return false;
    }

    if (fee == 0) {
        std::cout << "invalid fee: fee must be greater than 0" << std::endl;
        return false;
    }

    return true;
}

bool transaction_builder::build(transaction& tx) {
    bool done = false;
    temp = transaction(0x01, 0);
    temp.set_network(testnet ? "testnet" : "mainnet");
    while (TRANSACTION_LOOP) {
        std::string tcommand;
        std::cout << "[t] > ";
        std::cout.flush();
        std::cin >> tcommand;

#ifdef DEBUG
        std::cout << tcommand << std::endl;
#endif

        switch (get_tcommand(tcommand)) {
            case tcommands::T_ADD_INPUT:
                add_input();
                break;
            case tcommands::T_ADD_OUTPUT:
                add_output();
                break;
            case tcommands::T_ADD_MESSAGE:
                add_message();
                break;
            case tcommands::T_DONE: {
                TRANSACTION_LOOP = false;

                uint64_t tx_fee;
                if (!get_fee(tx_fee)) {
                    TRANSACTION_LOOP = true;
                    break;
                }

                finalize_tx(tx_fee);

                tx = temp;
                done = true;
                break;
            }
            case tcommands::T_REFRESH:
                std::cout << "  Refreshing wallet... ";
                std::cout.flush();
                wallet->refresh();
                std::cout << "done" << std::endl;
                break;
            case tcommands::T_EXIT:
                cin_clear_line();
                TRANSACTION_LOOP = false;
                break;
            case tcommands::T_HELP:
            default:
                cin_clear_line();
                print_thelp();
        }
    }

    return done;
}

void transaction_builder::print_thelp() {
    std::cout << "Support transaction building commands:" << std::endl;
    for (const auto& str : thelp) {
        std::cout << "  " << str << std::endl;
    }
    std::cout << std::endl;
    for (const auto& str : tnote) {
        std::cout << "  " << str << std::endl;
    }
}

tcommands transaction_builder::get_tcommand(const std::string& arg) {
    if (support_tcommands.find(arg) == support_tcommands.end()) {
        std::cout << "Unknown command" << std::endl;
        return tcommands::T_HELP;
    }

    return support_tcommands.find(arg)->second;
}

bool valid(const std::string& str, uint32_t size) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    std::string size_str = std::to_string(size);

    return str.length() < size_str.length() ||
           (str.length() == size_str.length() && str <= size_str);
}

void transaction_builder::add_input() {
    std::cout << "  Unspent transactions:" << std::endl;
    for (uint32_t i = 0; i < unspents.size(); i++) {
        if (std::find(choosen_txs.begin(), choosen_txs.end(), i) !=
                choosen_txs.end()) {
            continue;
        }

        std::cout << format("  (%u): %s", i + 1, unspents[i].to_string().c_str())
                  << std::endl;
    }

    std::cout << "  Choose a transaction to use as input (0 to cancel): ";
    cin_clear_line();
    std::string choose;
    std::cin >> choose;

    if (!valid(choose, unspents.size())) {
        std::cout << "  Invalid input" << std::endl;
    } else {
        uint32_t picked = std::stoi(choose);

        if (picked == 0) {
            return;
        }

        input in(unspents[picked - 1].previous_tx);
        in.set_sequence(0xffffffff);
        temp.add_input(in);

        choosen_txs.push_back(picked - 1);
        std::cout << format("  Picked transaction #%u", picked) << std::endl;
        std::cout << format("  You can spend up to: %s BTC",
                            get_total_avail_fund().c_str()) << std::endl;
    }
}

void transaction_builder::add_output() {
    std::cout << "  Enter receive address: ";
    std::string encoded_address;
    std::cin >> encoded_address;
    cin_clear_line();

    payment_address address;
    try {
        address = payment_address(encoded_address);
    } catch (std::invalid_argument& e) {
        std::cout << e.what() << std::endl;
        return;
    }

    std::string btc_left;

    uint64_t fund;
    bc::decode_base10(fund, get_total_avail_fund(), 8);
    uint64_t spent = temp.get_total_spends();

    uint64_t available = fund - spent;

    std::string available_btc = bc::encode_base10(available, 8);

    std::cout << "  Left over BTC to spend: " << available_btc << " BTC"
              << std::endl;

    std::string btc;
    std::cout << "  Enter amount to send (in BTC): ";
    std::cin >> btc;

    uint64_t satoshi;
    if (!bc::decode_base10(satoshi, btc, 8)) {
        std::cout << "invalid fund" << std::endl;
        return;
    }

    if (available < satoshi) {
        std::cout << "insufficient fund" << std::endl;
        return;
    }

    temp.add_output({address, satoshi});
}

void transaction_builder::add_message() {
    std::cout << "  Enter your message: ";
    cin_clear_line();
    std::string message;
    std::getline(std::cin, message);
    temp.set_message(message);
}

std::string transaction_builder::get_total_avail_fund() {
    uint64_t total_picked= 0;
    for (auto i : choosen_txs) {
        total_picked += unspents[i].value;
    }
    return bc::encode_base10(total_picked, 8);
}

void transaction_builder::sign_tx() {
    for (uint32_t i = 0; i < choosen_txs.size(); i++) {
        hd_private private_key = unspents[i].priv;
        temp.sign(i, private_key.to_public().get_point(),
                  payment_address(private_key).get_hash(),
                  private_key.get_secret(), 0x01);
    }
}

uint64_t transaction_builder::calculate_max_fee() {
    transaction tx = temp;

    output change(wallet->get_new_payment_address(false), 0);
    tx.add_output(change);

    for (uint32_t i = 0; i < choosen_txs.size(); i++) {
        hd_private private_key = unspents[i].priv;
        tx.sign(i, private_key.to_public().get_point(),
                payment_address(private_key).get_hash(),
                private_key.get_secret(), 0x01);
    }

    uint32_t bytes = tx.to_data().size();
    uint64_t diff = tx.get_total_fund() - tx.get_total_spends();

    return diff / bytes;
}

bool transaction_builder::get_fee(uint64_t& fee) {
    if (temp.get_total_fund() < temp.get_total_spends()) {
        std::cout << format("insufficient fund (in = %llu, out = %llu)",
                            temp.get_total_fund(), temp.get_total_spends())
                  << std::endl;
        return false;
    }

    uint64_t max_fee = calculate_max_fee();
    if (max_fee == 0) {
        std::cout << "left over fund is insufficient for fee" << std::endl;
        return false;
    }

    std::cout << "  Fastest fee (per byte) is " << get_fastest_fee()
              << std::endl << "  Your maximum fee (per byte) is " << max_fee
              << std::endl;

    std::cout << "  Please enter your transaction fee (per byte): ";
    std::string fee_str;
    std::cin >> fee_str;

    uint64_t fee_per_byte;
    if (!verify_fee(fee_per_byte, fee_str)) {
        return false;
    }

    if (fee_per_byte > max_fee) {
        std::cout << format("fee exceeds maximum fee (%llu)",
                            max_fee) << std::endl;
        return false;
    }

    fee = calculate_tx_fee(fee_per_byte);

    return true;
}

uint64_t transaction_builder::calculate_tx_fee(uint64_t fee_per_byte) {
    transaction tx = temp;

    output change(wallet->get_new_payment_address(false), 0);
    tx.add_output(change);

    for (uint32_t i = 0; i < choosen_txs.size(); i++) {
        hd_private private_key = unspents[i].priv;
        tx.sign(i, private_key.to_public().get_point(),
                payment_address(private_key).get_hash(),
                private_key.get_secret(), 0x01);
    }

    return fee_per_byte * tx.to_data().size();
}

void transaction_builder::finalize_tx(uint64_t tx_fee) {
    std::cout << "  Your transaction fee is: " << tx_fee << " satoshis"
              << std::endl;
    std::cout << "  Making change output... ";
    std::cout.flush();

    uint64_t change = temp.get_total_fund() - temp.get_total_spends() - tx_fee;
    temp.add_output({wallet->get_new_payment_address(), change});

    std::cout << "done" << std::endl << "  Signing transaction... ";
    std::cout.flush();
    sign_tx();
    std::cout << "done" << std::endl;
}
