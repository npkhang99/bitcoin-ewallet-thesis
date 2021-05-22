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
     * Generate wallet from given mnemonic
     * @param mnemonic mnemonic to generate wallet from
     */
    hd_wallet(const bc::wallet::word_list& mnemonic);

    /**
     * Dumps wallet data for debugging purposes
     */
    void dumps();

    /**
     * BIP39 optional passphrase for seed generation process
     * @param passphrase the passphrase
     */
    void set_passphrase(const std::string& passphrase);

    /**
     * Get master public key
     * @return wallet::hd_public master public key
     */
    bc::wallet::payment_address get_address();

    /**
     * BIP-32 HD Wallet private key derivation
     * @param path key derivation path
     * @return wallet::hd_private children private key
     */
    bc::wallet::hd_private derive_private(const std::vector<int>& path);

    /**
     * BIP-32 HD Wallet public key derivation
     * @param path key derivation path
     * @return wallet::hd_public children public key
     */
    bc::wallet::hd_public derive_public(const std::vector<int>& path);

#ifdef DEBUG

    bc::data_chunk get_entropy() {
        return _entropy;
    }

    bc::data_chunk get_seed() {
        return _seed;
    }

    bc::wallet::word_list get_mnemonic() {
        return _mnemonic;
    }

    bc::wallet::hd_private get_master_private() {
        return _master_private;
    }

    bc::wallet::hd_public get_master_public() {
        return _master_public;
    }

#endif

private:
    bc::data_chunk _entropy;
    bc::data_chunk _seed;
    bc::wallet::word_list _mnemonic;
    bc::wallet::hd_private _master_private;
    bc::wallet::hd_public _master_public;
    std::string _passphrase;

    const std::size_t DEFAULT_ENTROPY_BITS = 128;
    const std::string PASSPHRASE_PREFIX = "mnemonic";

    void generate_mnemonic();

    void generate_master_keys();
};


#endif //LIBBITCOIN_THESIS_HD_WALLET_H
