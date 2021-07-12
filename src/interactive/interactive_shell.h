#ifndef LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H
#define LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#include "data.h"
#include "utilities.h"
#include "transaction_builder.h"

#include "../wallet/hd_wallet.h"

/// An interactive shell to run program
class interactive_shell {
public:
    interactive_shell();

    int run(int argc, const char* argv[]);

private:
    bool init_wallet(const std::vector<std::string>& mnemonic = {});

    void load_wallet();

    bool testnet;
    hd_wallet* wallet;
    bool APP_LOOP;

    void list_transaction_hist();
    void refresh_wallet();
};


#endif //LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H
