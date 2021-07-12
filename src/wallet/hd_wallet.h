#ifndef LIBBITCOIN_THESIS_HD_WALLET_H
#define LIBBITCOIN_THESIS_HD_WALLET_H

#include <iostream>
#include <string>
#include <bitcoin/bitcoin.hpp>

#include "../client/client_utilities.h"

#include "hd_public.h"
#include "hd_private.h"
#include "payment_address.h"
#include "trans_info.h"

/**
 * HD Wallet implementation
 */
class hd_wallet {
public:
    /// Constructors
    hd_wallet();
    hd_wallet(bool testnet);
    hd_wallet(const bc::data_chunk& entropy, bool testnet = false);
    hd_wallet(const std::string& mnemonic_sentence, bool testnet = false);
    hd_wallet(const bc::wallet::word_list& mnemonic, bool testnet = false);

    /**
     * BIP39 optional passphrase for seed generation process
     * @param passphrase the passphrase
     */
    void set_passphrase(const std::string& passphrase);

    void set_base_derive_path(const std::vector<uint32_t>& base_derive_path);

    /**
     * BIP-32 HD Wallet private key derivation
     * @param path key derivation path
     * @return wallet::hd_private children private key
     */
    hd_private derive_private(const std::vector<uint32_t>& path);

    /**
     * BIP-32 HD Wallet public key derivation
     * @param path key derivation path
     * @return wallet::hd_public children public key
     */
    hd_public derive_public(const std::vector<uint32_t>& path);

    /**
     * Explore the HD wallet for used keys
     */
    void explore();
    void refresh();

    /**
     * Get a new unused Bitcoin payment address
     * @param increase should increase child index or not
     * @return unused payment address
     */
    payment_address get_new_payment_address(bool increase = true);

    /**
     * Get balance of all address in wallet
     * @return balance in BTC
     */
    std::string get_balance();

    /**
     * Get all used payment addresses
     * @return used payment addresses
     */
    std::vector<payment_address> get_used_payment_addresses();

    /**
     * Get all transactions have been made with this wallet addresses
     * @return a vector of transaction info
     */
    std::vector<trans_info> get_unspent_txs();

#ifdef DEBUG
    /**
     * Dumps wallet data for debugging purposes
     */
    void dumps() const {
        std::cout << "Entropy: " << bc::encode_base16(_entropy) << std::endl;
        std::cout << "Mnemonic: " << bc::join(_mnemonic) << std::endl;
        std::cout << "Passphrase: " << (_passphrase.empty() ? "(none)" : _passphrase)
                  << std::endl;
        std::cout << "Master private key: " << _master_private.encoded()
                  << std::endl;
        std::cout << "Master public key: " << _master_public.encoded() << std::endl;
    }

    bc::data_chunk get_entropy() const {
        return _entropy;
    }

    bc::data_chunk get_seed() const {
        return _seed;
    }

    bc::wallet::word_list get_mnemonic() const {
        return _mnemonic;
    }

    hd_private get_master_private() const {
        return _master_private;
    }

    hd_public get_master_public() const {
        return _master_public;
    }
#endif

private:
    bool _testnet;

    bc::data_chunk _entropy;
    bc::data_chunk _seed;
    bc::wallet::word_list _mnemonic;
    hd_private _master_private;
    hd_public _master_public;
    std::string _passphrase;

    // BIP-0044 base derive path
    std::vector<uint32_t> _base_derive_path = {
        hd_private::first_hardened_key + 44,    // Purpose
        hd_private::first_hardened_key,         // Coin
        hd_private::first_hardened_key,         // Account
        0                                       // External/Internal
    };

    uint32_t _first_unused = 0;
    uint32_t _next_child_key_index = 0;

    const std::size_t DEFAULT_ENTROPY_BITS = 256;
    const std::string PASSPHRASE_PREFIX = "mnemonic";

    void generate_mnemonic();

    void generate_master_keys();

    void explore(uint32_t begin);

    void set_testnet(bool testnet);
};


#endif //LIBBITCOIN_THESIS_HD_WALLET_H
