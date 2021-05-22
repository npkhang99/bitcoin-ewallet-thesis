#include "hd_private.h"

const uint32_t hd_private::mainnet = 76066276;
const uint32_t hd_private::testnet = 70615956;

hd_private::hd_private() {
}

hd_private::hd_private(const bc::data_chunk& seed, uint32_t version) {
    // BIP32 magic key for HMAC-SHA512
    const auto magic = bc::to_chunk("Bitcoin seed");

    const auto hash_output = bc::hmac_sha512_hash(seed, magic);
    const auto split = bc::split(hash_output);

    if (!bc::verify(split.left)) {
        throw std::range_error("Master key is invalid (is 0 or >= n)");
    }

    _secret = split.left;
    _chain_code = split.right;
    _lineage.version = version;
}

hd_public hd_private::to_public() const {
    return {*this};
}

const bc::byte_array<32>& hd_private::get_secret() const {
    return _secret;
}

const bc::byte_array<32>& hd_private::get_chain() const {
    return _chain_code;
}

const hd_lineage& hd_private::get_lineage() const {
    return _lineage;
}

std::string hd_private::encoded() const {
    bc::data_chunk serialized;
    bc::extend_data(serialized, bc::to_big_endian(_lineage.version));
    bc::extend_data(serialized, bc::to_array(_lineage.depth));
    bc::extend_data(serialized, bc::to_big_endian(_lineage.fingerprint));
    bc::extend_data(serialized, bc::to_big_endian(_lineage.index));
    bc::extend_data(serialized, _chain_code);
    bc::extend_data(serialized, bc::one_byte{0});
    bc::extend_data(serialized, _secret);
    bc::append_checksum(serialized);

    std::string to_str = bc::encode_base58(serialized);

    return to_str;
}

hd_private hd_private::derive_private(uint32_t index) const {
    bc::data_chunk data;
    if (index >= first_hardened_key) { // hardened child key
        data.push_back(0); // 0x00 padding
        bc::extend_data(data, _secret);
    } else { // normal child key
        bc::extend_data(data, secp256k1_point(_secret));
    }

    bc::extend_data(data, bc::to_big_endian(index));

    auto tmp = bc::split(bc::hmac_sha512_hash(data, _chain_code));

    auto child_secret = _secret;
    if (!bc::ec_add(child_secret, tmp.left)) {
        return {};
    }

    if (_lineage.depth == 255) {
        return {};
    }

    hd_lineage lineage{
            _lineage.version,
            static_cast<uint8_t>(_lineage.depth + 1),
            fingerprint(),
            index,
    };

    return {child_secret, lineage, tmp.right};
}

hd_public hd_private::derive_public(uint32_t index) const {
    return derive_private(index).to_public();
}

uint32_t hd_private::fingerprint() const {
    uint32_t out = 0;
    auto data = bc::bitcoin_short_hash(secp256k1_point(_secret));
    for (int i = 0; i < 4; i++) {
        out <<= 8;
        out += data.at(i);
    }
    return out;
}

hd_private::hd_private(const libbitcoin::byte_array<32>& secret,
                       const hd_lineage& lineage,
                       const libbitcoin::byte_array<32>& chain_code) :
        _secret(secret), _lineage(lineage), _chain_code(chain_code) {}
