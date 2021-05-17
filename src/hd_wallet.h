#ifndef LIBBITCOIN_THESIS_HD_WALLET_H
#define LIBBITCOIN_THESIS_HD_WALLET_H

#include <iostream>
#include <string>
#include <bitcoin/bitcoin.hpp>

/**
 * HD Wallet implementation
 */
class hd_wallet {
public:
    hd_wallet();
    hd_wallet(const bc::data_chunk& entropy);
    hd_wallet(const bc::wallet::word_list& mnemonic);

    void dumps();

private:
    bc::data_chunk _entropy;
    bc::data_chunk _seed;
    bc::wallet::word_list _mnemonic;
    bc::wallet::hd_private _root_private;
    bc::wallet::hd_public _root_public;

    void init_from_entropy();
    void init_from_mnemonic();
};


#endif //LIBBITCOIN_THESIS_HD_WALLET_H
