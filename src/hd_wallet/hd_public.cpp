#include "hd_public.h"
#include "hd_private.h"

const uint32_t hd_public::mainnet = 76067358;
const uint32_t hd_public::testnet = 70617039;

hd_public::hd_public() {
    _valid = false;
}

hd_public::hd_public(const hd_private& priv) {
    _point = secp256k1_point(priv.get_secret());
    _chain_code = priv.get_chain();
    _lineage = priv.get_lineage();

    if (priv.get_lineage().version == hd_private::mainnet) {
        _lineage.version = mainnet;
    } else {
        _lineage.version = testnet;
    }
}

const bc::byte_array<33>& hd_public::get_point() const {
    return _point;
}

std::string hd_public::encoded() const {
    bc::data_chunk serialized;
    bc::extend_data(serialized, bc::to_big_endian(_lineage.version));
    bc::extend_data(serialized, bc::to_array(_lineage.depth));
    bc::extend_data(serialized, bc::to_big_endian(_lineage.fingerprint));
    bc::extend_data(serialized, bc::to_big_endian(_lineage.index));
    bc::extend_data(serialized, _chain_code);
    bc::extend_data(serialized, _point);
    bc::append_checksum(serialized);

    std::string to_str = bc::encode_base58(serialized);

    return to_str;
}

hd_public hd_public::derive_public(uint32_t index) const {
    if (index >= first_hardened_key) { // hardened child key
        throw std::range_error("Cannot derive hardened child public from public parent key");
    }

    auto data = bc::splice(_point, bc::to_big_endian(index));
    auto tmp = bc::split(bc::hmac_sha512_hash(data, _chain_code));

    // The returned child key Ki is point(parse256(IL)) + Kpar
    auto child_point = _point;
    if (!bc::ec_add(child_point, tmp.left)) {
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

    return {child_point, lineage, tmp.right};
}

uint32_t hd_public::fingerprint() const {
    uint32_t out = 0;
    auto data = bc::bitcoin_short_hash(_point);
    for (int i = 0; i < 4; i++) {
        out <<= 8;
        out += data.at(i);
    }
    return out;
}

hd_public::hd_public(const libbitcoin::byte_array<33>& point,
                     const hd_lineage& lineage,
                     const libbitcoin::byte_array<32>& chain_code) :
        _point(point), _lineage(lineage), _chain_code(chain_code) {}
