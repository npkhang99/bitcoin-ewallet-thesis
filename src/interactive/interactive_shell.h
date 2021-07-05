#ifndef LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H
#define LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H

#include <iostream>
#include <vector>
#include <string>

#include "../wallet/hd_wallet.h"

#include "data.h"
#include "utilities.h"

class interactive_shell {
public:
    interactive_shell();

    int run(int argc, const char* argv[]);

private:
    bool init_wallet(const std::vector<std::string>& mnemonic = {},
                     const std::string& passphrase = "");

    void load_wallet();

    bool testnet;
    hd_wallet* wallet;
    bool APP_LOOP;
};


#endif //LIBBITCOIN_THESIS_INTERACTIVE_SHELL_H
