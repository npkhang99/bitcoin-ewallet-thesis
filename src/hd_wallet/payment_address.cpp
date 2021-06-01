#include "payment_address.h"

const uint8_t payment_address::mainnet_p2kh = 0x00;
const uint8_t payment_address::testnet_p2kh = 0x6f;

// payment_size (byte) = 1 (version) + 20 (hash160 size) + 4 (4-byte checksum)
const uint32_t payment_address::payment_size = 1u + bc::short_hash_size + bc::checksum_size;

payment_address::payment_address() : _version(0), _hash(bc::null_short_hash) {}

payment_address::payment_address(const payment_address& o) :
        _version(o._version), _hash(o._hash) {}

payment_address::payment_address(const hd_public& pubkey, uint8_t version) :
        _version(version), _hash(bc::bitcoin_short_hash(pubkey.get_point())) {}

payment_address::payment_address(const hd_private& privkey, uint8_t version) :
        _version(version),
        _hash(bc::bitcoin_short_hash(privkey.to_public().get_point())) {}

payment_address::payment_address(const bc::byte_array<33>& point, uint8_t version)
        : _version(version), _hash(bc::bitcoin_short_hash(point)) {}

payment_address::payment_address(const std::string& address) {
    bc::byte_array<payment_size> raw;

    if (!bc::decode_base58(raw, address) || !bc::verify_checksum(raw)) {
        throw std::invalid_argument("invalid address");
    }

    _version = raw[0];
    _hash = bc::slice<1, 1 + bc::short_hash_size>(raw);
}

std::string payment_address::encoded() const {
    if (_hash == bc::null_short_hash) {
        throw std::bad_function_call();
    }

    bc::data_chunk raw;
    raw.push_back(_version); // version 0: bitcoin address
    bc::extend_data(raw, _hash); // append public key hash
    bc::append_checksum(raw); // calculate and append 4 bytes checksum

    std::string bitcoin_address = bc::encode_base58(raw);

#ifdef DEBUG
    // validate with library's function
    bc::wallet::payment_address control = bc::wallet::payment_address(_hash);
    assert(control.encoded() == bitcoin_address);
#endif

    return bitcoin_address;
}

const bc::short_hash& payment_address::get_hash() const {
    return _hash;
}
