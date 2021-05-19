#include <iostream>
#include <string>
#include <secp256k1.h>

#include "hd_wallet.h"
#include "functions.h"

void test_address() {
    bc::ec_secret secret_ec = generate_secret(
            "067375cf2f49e9bc7b759acdcc6b9cd43326a550415eff5ba8d5e266d7adfca3");
    std::cout << "EC secret:" << std::endl << bc::encode_base16(secret_ec)
              << std::endl;

    auto key_pair = generate_wallet_ec_key_pair(secret_ec);
    std::cout << "EC private key:" << std::endl << key_pair.second.encoded()
              << std::endl;
    std::cout << "EC public key:" << std::endl << key_pair.first.encoded()
              << std::endl;

    std::string address = generate_address(key_pair.first);
    std::cout << "Bitcoin address:" << std::endl << address << std::endl;
}

void test_hd_wallet() {
    {
        hd_wallet wallet;
        wallet.dumps();
    }

#ifdef DEBUG
    std::cout << "====================" << std::endl;
    std::cout << "Testing with Mastering bitcoin book's examples..."
              << std::endl;

    {
        bc::data_chunk entropy;
        bc::wallet::word_list expect_mnemonic;
        bc::data_chunk expect_seed;

        bc::decode_base16(entropy, "0c1e24e5917779d297e14d45f14e1a1a");
        hd_wallet wallet(entropy);

        expect_mnemonic = bc::split(
                "army van defense carry jealous true garbage claim echo media make crunch");
        bc::decode_base16(expect_seed,
                          "5b56c417303faa3fcba7e57400e120a0ca83ec5a4fc9ffba757fbe63fbd77a89a1a3be4c67196f57c39a88b76373733891bfaba16ed27a813ceed498804c0570");

        assert(wallet.get_mnemonic() == expect_mnemonic);
        assert(wallet.get_seed() == expect_seed);

        std::cout << "Test #1 passed" << std::endl;
    }

    {
        bc::data_chunk entropy;
        bc::wallet::word_list expect_mnemonic;
        bc::data_chunk expect_seed;

        bc::decode_base16(entropy, "0c1e24e5917779d297e14d45f14e1a1a");
        hd_wallet wallet(entropy);
        wallet.set_passphrase("SuperDuperSecret");

        expect_mnemonic = bc::split(
                "army van defense carry jealous true garbage claim echo media make crunch");
        bc::decode_base16(expect_seed,
                          "3b5df16df2157104cfdd22830162a5e170c0161653e3afe6c88defeefb0818c793dbb28ab3ab091897d0715861dc8a18358f80b79d49acf64142ae57037d1d54");

        assert(wallet.get_mnemonic() == expect_mnemonic);
        assert(wallet.get_seed() == expect_seed);

        std::cout << "Test #2 passed" << std::endl;
    }

    {
        bc::data_chunk entropy;
        bc::wallet::word_list expect_mnemonic;
        bc::data_chunk expect_seed;

        bc::decode_base16(entropy,
                          "2041546864449caff939d32d574753fe684d3c947c3346713dd8423e74abcf8c");
        hd_wallet wallet(entropy);

        expect_mnemonic = bc::split(
                "cake apple borrow silk endorse fitness top denial coil riot stay wolf luggage oxygen faint major edit measure invite love trap field dilemma oblige");
        bc::decode_base16(expect_seed,
                          "3269bce2674acbd188d4f120072b13b088a0ecf87c6e4cae41657a0bb78f5315b33b3a04356e53d062e55f1e0deaa082df8d487381379df848a6ad7e98798404");

        assert(wallet.get_mnemonic() == expect_mnemonic);
        assert(wallet.get_seed() == expect_seed);

        std::cout << "Test #3 passed" << std::endl;
    }
#endif
}

void generate_ec_pubkey_secp256k1() {
    bc::data_chunk entropy;
    bc::decode_base16(entropy, "6b750e61ccf5ef5e208991bdc9259022");
    hd_wallet wallet(entropy);
    wallet.dumps();

//    std::cerr << bc::encode_base16((bc::ec_secret) wallet.get_master_private()) << std::endl;
//    std::cerr << bc::encode_base16(wallet.get_master_public().point()) << std::endl;

    secp256k1_context *context = secp256k1_context_create(
            SECP256K1_CONTEXT_VERIFY | SECP256K1_CONTEXT_SIGN);
    secp256k1_pubkey pubkey;

    bc::ec_secret secret = wallet.get_master_private().secret();
    std::cerr << "Master secret key: " << bc::encode_base16(secret)
              << std::endl;

    bool success = secp256k1_ec_pubkey_create(context, &pubkey, secret.data());
    if (!success) {
        std::cerr << "error creating pubkey" << std::endl;
    } else {
        std::uint8_t serialized_pub[33];
        std::size_t len = sizeof(serialized_pub);
        secp256k1_ec_pubkey_serialize(context, serialized_pub, &len, &pubkey,
                                      SECP256K1_EC_COMPRESSED);
        bc::data_chunk pub_chunk(serialized_pub, serialized_pub + len);
        std::cerr << "Got: " << bc::encode_base16(pub_chunk) << std::endl;

        std::cerr << "Checking correctness... ";
        if (bc::encode_base16(wallet.get_master_public().point()) ==
                bc::encode_base16(pub_chunk)) {
            std::cerr << "correct!" << std::endl;
        } else {
            std::cerr << "failed" << std::endl;
            std::cerr << "Expected result: "
                      << bc::encode_base16(wallet.get_master_public().point())
                      << std::endl;
        }
    }

    secp256k1_context_destroy(context);
}

int main() {
    test_address();
    return 0;
}
