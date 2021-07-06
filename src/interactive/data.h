#ifndef LIBBITCOIN_THESIS_DATA_H
#define LIBBITCOIN_THESIS_DATA_H

#include <map>
#include <string>

const static char* usage = "usage: %s [chain]\nOptions:\n"
                           "  chain: 'mainnet' (default) or 'testnet'";

const static char* interactive_usage = "Interactive shell usage:\n"
                                       "  <command> [args...]";

const static char* help[] = {
        "exit: exit the program",
        "help: print this usage message",
        "newwallet: create a new HD wallet",
        "loadwallet <mnemonic>: initialize a new HD wallet from <mnemonic>",
        "getbalance: get current wallet balance",
        "newaddress: get new Bitcoin payment address",
        "history: list all transactions history of this wallet"
};

enum commands {
    EXIT,
    HELP,
    NEW_WALLET,
    LOAD_WALLET,
    GET_BALANCE,
    HISTORY,
    NEW_TRANSACTION,
    NEW_ADDRESS
};

const std::map<std::string, commands> support_commands = {
        {"exit",           EXIT},
        {"help",           HELP},
        {"newwallet",      NEW_WALLET},
        {"loadwallet",     LOAD_WALLET},
        {"getbalance",     GET_BALANCE},
        {"history",        HISTORY},
        {"newtransaction", NEW_TRANSACTION},
        {"newaddress",     NEW_ADDRESS}
};

#endif //LIBBITCOIN_THESIS_DATA_H
