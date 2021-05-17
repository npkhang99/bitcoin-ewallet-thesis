#include "hd_wallet.h"

hd_wallet::hd_wallet() {
    _entropy = bc::data_chunk(_ENTROPY_BITS / 8);
    bc::pseudo_random_fill(_entropy);
    init_from_entropy();
}

hd_wallet::hd_wallet(const bc::data_chunk &entropy) {
    _entropy = entropy;
    init_from_entropy();
}

hd_wallet::hd_wallet(const bc::wallet::word_list &mnemonic) {
    if (!bc::wallet::validate_mnemonic(mnemonic)) {
        throw std::invalid_argument("Mnemonic not valid");
    }
    _mnemonic = mnemonic;
    init_from_mnemonic();
}

void hd_wallet::dumps() {
    std::cout << "Entropy: " << bc::encode_base16(_entropy) << std::endl;
    std::cout << "Mnemonic: " << bc::join(_mnemonic) << std::endl;
    std::cout << "Root private key: " << _root_private.encoded() << std::endl;
    std::cout << "Root public key: " << _root_public.encoded() << std::endl;
}

std::string to_binary(uint8_t num) {
    std::string binary;
    for (int i = 7; i >= 0; i--) {
        binary.push_back(num >> i & 1 ? '1' : '0');
    }
    return binary;
}

void hd_wallet::init_from_entropy() {
    size_t entropy_bits = _entropy.size() * 8;
    size_t checksum_bits = entropy_bits / 32;
    size_t data_bits = entropy_bits + checksum_bits;
    size_t word_counts = data_bits / 11;

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
    for (size_t word = 0; word < word_counts; word++) {
        size_t position = std::stoi(data_str.substr(11 * word, 11), nullptr, 2);
        assert(position < bc::wallet::dictionary_size);
        _mnemonic.push_back(bip39_dictionary[position]);
    }

#ifdef DEBUG
    bc::wallet::word_list control = bc::wallet::create_mnemonic(_entropy);
    assert(_mnemonic == control);
#endif

    init_from_mnemonic();
}

void hd_wallet::init_from_mnemonic() {
    _seed = bc::to_chunk(bc::wallet::decode_mnemonic(_mnemonic));
    _root_private = bc::wallet::hd_private(_seed);
    _root_public = _root_private.to_public();
}
