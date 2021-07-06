#include "hd_wallet.h"

hd_wallet::hd_wallet() : hd_wallet(false) {}

hd_wallet::hd_wallet(bool testnet) {
    _entropy = bc::data_chunk(DEFAULT_ENTROPY_BITS / 8);
    bc::pseudo_random_fill(_entropy);
    _testnet = testnet;

    generate_mnemonic();
    generate_master_keys();
}

hd_wallet::hd_wallet(const bc::data_chunk& entropy, bool testnet) {
    _entropy = entropy;
    _testnet = testnet;

    generate_mnemonic();
    generate_master_keys();
}

hd_wallet::hd_wallet(const std::string& mnemonic_sentence, bool testnet)
        : hd_wallet(bc::split(mnemonic_sentence), testnet) {}

hd_wallet::hd_wallet(const bc::wallet::word_list& mnemonic, bool testnet) {
    if (!bc::wallet::validate_mnemonic(mnemonic)) {
        throw std::invalid_argument("Mnemonic not valid");
    }
    _mnemonic = mnemonic;
    _testnet = testnet;

    generate_master_keys();
}

hd_private hd_wallet::derive_private(const std::vector<uint32_t>& path) {
    hd_private child_key = _master_private;
    for (uint32_t index : path) {
        child_key = child_key.derive_private(index);
    }
    return child_key;
}

hd_public hd_wallet::derive_public(const std::vector<uint32_t>& path) {
    hd_public child_key = _master_public;
    for (uint32_t index : path) {
        child_key = child_key.derive_public(index);
    }
    return child_key;
}

void hd_wallet::set_passphrase(const std::string& passphrase) {
    _passphrase = passphrase;
    generate_master_keys();
}

std::string to_binary(uint8_t num) {
    std::string binary;
    for (int8_t i = 7; i >= 0; i--) {
        binary.push_back(num >> i & 1 ? '1' : '0');
    }
    return binary;
}

void hd_wallet::generate_mnemonic() {
    std::size_t entropy_bits = _entropy.size() * 8;
    std::size_t checksum_bits = entropy_bits / 32;
    std::size_t data_bits = entropy_bits + checksum_bits;
    std::size_t word_counts = data_bits / 11;

    bc::data_chunk data = _entropy;
    bc::extend_data(data, bc::sha256_hash_chunk(_entropy));

    std::string data_str;
    for (auto d : data) {
        std::string byte_str = to_binary(d);
        data_str += byte_str;

        if (data_str.size() >= data_bits) {
            break;
        }
    }

    bc::wallet::dictionary bip39_dictionary = bc::wallet::language::en;

    _mnemonic.clear();
    for (std::size_t word = 0; word < word_counts; word++) {
        std::size_t position = std::stoi(data_str.substr(11 * word, 11),
                                         nullptr, 2);
        assert(position < bc::wallet::dictionary_size);
        _mnemonic.push_back(bip39_dictionary[position]);
    }

#ifdef DEBUG
    bc::wallet::word_list control = bc::wallet::create_mnemonic(_entropy);
    assert(_mnemonic == control);
#endif
}

void hd_wallet::generate_master_keys() {
    _seed = bc::to_chunk(
                bc::pkcs5_pbkdf2_hmac_sha512(
                        bc::to_chunk(bc::join(_mnemonic)),
                        bc::to_chunk(PASSPHRASE_PREFIX + _passphrase),
                        2048
                )
            );

#ifdef DEBUG
    bc::data_chunk control =
            bc::to_chunk(bc::wallet::decode_mnemonic(_mnemonic, _passphrase));
    assert(_seed == control);
#endif

    if (_testnet) {
        _master_private = hd_private(_seed, hd_private::testnet);
    } else {
        _master_private = hd_private(_seed);
    }

    _master_public = _master_private.to_public();

    std::cout << "Wallet successfully created" << std::endl;
    std::cout << "Your mnemonic is:" << std::endl;
    std::cout << bc::join(_mnemonic) << std::endl;
}

void hd_wallet::set_base_derive_path(const std::vector<uint32_t>& base_derive_path) {
    _base_derive_path = base_derive_path;
}

void hd_wallet::explore() {
    explore(0);
}

void hd_wallet::refresh() {
    explore(_first_unused);
}

void hd_wallet::explore(uint32_t begin) {
    hd_private base_priv = derive_private(_base_derive_path);

    uint32_t index;
    uint32_t consecutive_unused_count = 0;

    for (index = begin; consecutive_unused_count < 20; index++) {
        payment_address address(base_priv.derive_private(index));

        Json::Value info;
        client::get_address_info(info, address.encoded(),
                                 _testnet ? client::testnet : client::mainnet);

        if (info["total_txs"].asUInt() == 0) {
            consecutive_unused_count++;
        } else {
            _first_unused += consecutive_unused_count + 1;
            consecutive_unused_count = 0;
        }
    }

    _next_child_key_index = _first_unused;
}

payment_address hd_wallet::get_new_payment_address(bool increase) {
    hd_private base_priv = derive_private(_base_derive_path);
    payment_address address(base_priv.derive_private(_next_child_key_index));

    if (increase) {
        _next_child_key_index++;
    }

    return address;
}

std::string hd_wallet::get_balance() {
    hd_private base_priv = derive_private(_base_derive_path);
    uint64_t total_balance = 0;
    uint64_t available = 0;
    uint64_t pending = 0;

    for (uint32_t index = 0; index < _next_child_key_index; index++) {
        payment_address address(base_priv.derive_private(index));

        Json::Value info;
        client::get_address_info(info, address.encoded(),
                                _testnet ? client::testnet : client::mainnet);

        uint64_t balance;

        bc::decode_base10(balance, info["balance"].asString(), 8);
        available += balance;

        bc::decode_base10(balance, info["pending_value"].asString(), 8);
        pending += balance;
    }

    std::cout << "  Available fund: " << bc::encode_base10(available, 8) << " BTC" << std::endl;
    std::cout << "  Pending fund: " << bc::encode_base10(pending, 8) << " BTC" << std::endl;

    total_balance = available + pending;
    std::cout << "  Total fund: " << bc::encode_base10(total_balance, 8) << " BTC" << std::endl;

    return bc::encode_base10(available, 8);
}

std::vector<payment_address> hd_wallet::get_used_payment_addresses() {
    hd_private base_priv = derive_private(_base_derive_path);
    std::vector<payment_address> used;
    for (uint32_t index = 0; index < _first_unused; index++) {
        used.emplace_back(base_priv.derive_private(index));
    }
    return used;
}

std::vector<trans_info> hd_wallet::get_unspent_txs() {
    hd_private base_priv = derive_private(_base_derive_path);
    std::vector<trans_info> transactions;

    for (uint32_t index = 0; index < _first_unused; index++) {
        payment_address address(base_priv.derive_private(index));
        Json::Value unspents;
        client::get_unspent_txs(unspents, address.encoded(),
                                _testnet ? client::testnet : client::mainnet);

        for (const auto& tx : unspents["txs"]) {
            bc::hash_digest tx_hash;
            bc::decode_hash(tx_hash, tx["txid"].asString());
            bc::chain::output_point op(tx_hash, tx["output_no"].asInt64());

            uint64_t value;
            bc::decode_base10(value, tx["value"].asString(), 8);

            transactions.push_back({base_priv.derive_private(index), op, value});
        }
    }

    return transactions;
}
