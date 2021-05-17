#include "hd_wallet.h"

hd_wallet::hd_wallet() {
    _entropy = bc::data_chunk(16);
    bc::pseudo_random_fill(_entropy);
    init_from_entropy();
}

hd_wallet::hd_wallet(const bc::data_chunk& entropy) {
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
    std::cout << "Mnemonic: " << bc::join(_mnemonic) << std::endl;
    std::cout << "Root private key: " << _root_private.encoded() << std::endl;
    std::cout << "Root public key: " << _root_public.encoded() << std::endl;
}

void hd_wallet::init_from_entropy() {
    _mnemonic = bc::wallet::create_mnemonic(_entropy);
    init_from_mnemonic();
}

void hd_wallet::init_from_mnemonic() {
    _seed = bc::to_chunk(bc::wallet::decode_mnemonic(_mnemonic));
    _root_private = bc::wallet::hd_private(_seed);
    _root_public = _root_private.to_public();
}
