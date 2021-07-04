#include "utilities.h"

std::string format(const char* fmt, ...) {
    char buffer[(1u << 16) - 1];
    va_list ap;
    va_start(ap, fmt);
    std::vsprintf(buffer, fmt, ap);
    va_end(ap);
    return std::string(buffer);
}

bool need_wallet(commands cmd) {
    if (cmd == commands::HELP || cmd == commands::LOAD_WALLET ||
        cmd == commands::NEW_WALLET || cmd == commands::EXIT) {
        return false;
    }

    return true;
}

commands get_command(const std::string& arg, const hd_wallet* wallet) {
    if (support_commands.find(arg) == support_commands.end()) {
        return commands::HELP;
    }

    if (wallet == nullptr && need_wallet(support_commands.find(arg)->second)) {
        std::cerr << "you need to create or load a wallet before using this command"
                  << std::endl;
        return commands::HELP;
    }

    return support_commands.find(arg)->second;
}
