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
    /**
     * Default constructor to generate random seed wallet
     */
    hd_wallet();

    /**
     * Generate wallet from given entropy
     * @param entropy entropy to generate wallet from
     */
    hd_wallet(const bc::data_chunk& entropy);

    /**
     * Generate wallet from givem mnemonic
     * @param mnemonic mnemonic to generate wallet from
     */
    hd_wallet(const bc::wallet::word_list& mnemonic);

    /**
     * Dumps wallet data for debugging purposes
     */
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
