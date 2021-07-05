#ifndef LIBBITCOIN_THESIS_TRANSACTION_BUILDER_H
#define LIBBITCOIN_THESIS_TRANSACTION_BUILDER_H

#include <string>
#include <cctype>
#include <algorithm>

#include <bitcoin/bitcoin.hpp>

#include "utilities.h"

#include "../chain/transaction.h"

#include "../wallet/hd_wallet.h"
#include "../client/client_utilities.h"

enum tcommands {
    T_HELP,
    T_DONE,
    T_EXIT,
    T_ADD_INPUT,
    T_ADD_OUTPUT
};

const static char* thelp[] = {
        "exit: cancel transaction",
        "help: print this usage message",
        "done: confirm transaction",
        "addinput: add an input",
        "addoutput: add an output"
};

const std::map<std::string, tcommands> support_tcommands = {
        {"help", T_HELP},
        {"done", T_DONE},
        {"exit", T_EXIT},
        {"addinput", T_ADD_INPUT},
        {"addoutput", T_ADD_OUTPUT}
};

/// An interactive shell for building transaction
class transaction_builder {
public:
    transaction_builder(hd_wallet* wallet, bool testnet);

    /**
     * Start sub-shell
     */
    bool build(transaction& tx);

private:
    hd_wallet* wallet;

    bool testnet;
    bool TRANSACTION_LOOP = true;
    std::vector<trans_info> unspents;
    std::vector<uint32_t> choosen_txs;

    transaction temp;

    // private so one must provide a wallet to instantiate this class
    transaction_builder() {}

    static void print_thelp();
    static tcommands get_tcommand(const std::string& arg);

    void add_input();
    void add_output();

    std::string get_total_avail_fund();

    void sign_tx();
};


#endif //LIBBITCOIN_THESIS_TRANSACTION_BUILDER_H
