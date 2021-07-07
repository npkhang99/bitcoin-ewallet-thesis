#ifndef LIBBITCOIN_THESIS_TDATA_H
#define LIBBITCOIN_THESIS_TDATA_H

#include <map>
#include <string>

enum tcommands {
    T_HELP,
    T_DONE,
    T_EXIT,
    T_ADD_INPUT,
    T_ADD_OUTPUT,
    T_REFRESH,
    T_ADD_MESSAGE
};

const static char* tnote[] = {
        "Note: you do not need to specify the change output,",
        "it will automatically added when you finish the transaction",
        "(use the 'done' command)"
};

const static char* thelp[] = {
        "exit: cancel transaction",
        "help: print this usage message",
        "done: confirm transaction",
        "addinput: add an input to transaction",
        "addoutput: add an output to transaction",
        "addmessage: add a note to transaction",
        "refresh: refresh wallet"
};

const std::map<std::string, tcommands> support_tcommands = {
        {"help",      T_HELP},
        {"done",      T_DONE},
        {"exit",      T_EXIT},
        {"addinput",  T_ADD_INPUT},
        {"addoutput", T_ADD_OUTPUT},
        {"refresh",   T_REFRESH},
        {"addmessage", T_ADD_MESSAGE}
};

#endif //LIBBITCOIN_THESIS_TDATA_H
