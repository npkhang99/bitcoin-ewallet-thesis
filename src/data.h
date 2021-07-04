#ifndef LIBBITCOIN_THESIS_DATA_H
#define LIBBITCOIN_THESIS_DATA_H

#include <map>
#include <string>

enum commands {
    EXIT,
    HELP,
    NEW_WALLET,
    LOAD_WALLET,
    GET_BALANCE,
    LIST_TRANSACTIONS,
    NEW_TRANSACTION,
};

const std::map<std::string, commands> support_commands = {
        {"exit", EXIT},
        {"help", HELP},
        {"newwallet", NEW_WALLET},
        {"loadwallet", LOAD_WALLET},
        {"getbalance", GET_BALANCE},
        {"listtransactions", LIST_TRANSACTIONS},
        {"newtransaction", NEW_TRANSACTION}
};

const std::map<std::string, uint8_t> transaction_commands = {
        {"help", 0},
        {"done", 1},
        {"exit", 2},
        {"addoutput", 3},
};

#endif //LIBBITCOIN_THESIS_DATA_H
