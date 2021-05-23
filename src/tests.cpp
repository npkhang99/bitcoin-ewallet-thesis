#include "tests.h"

void test_address() {
    bc::ec_secret secret_ec = generate_secret(
            "067375cf2f49e9bc7b759acdcc6b9cd43326a550415eff5ba8d5e266d7adfca3");
    auto key_pair = generate_wallet_ec_key_pair(secret_ec);
    std::string address = generate_address(key_pair.first);
    std::cout << "Bitcoin address:" << std::endl << address << std::endl;
    std::cout << std::endl;
}

void test_hd_wallet() {
    {
        std::cout << "Random HD wallet generation:" << std::endl;
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
    std::cout << std::endl;
}

void test_hd_wallet_keys() {
    std::cout << "Testing wallet master keys generation..." << std::endl;

    hd_wallet wallet;
    // test private key generation
    hd_private hd_priv(wallet.get_seed());
    assert(wallet.get_master_private().get_secret() == hd_priv.get_secret());

    // test public key generation
    hd_public hd_pub(hd_priv);
    assert(wallet.get_master_public().get_point() == hd_pub.get_point());

    // make sure bitcoin payment address is also correct
    assert(bc::wallet::payment_address(wallet.get_master_public().get_point()) ==
           hd_pub.to_payment_address());

    std::cout << "Passed..." << std::endl << std::endl;
}

void bip32_test_vector_1() {
    std::cout << "Test vector #1..." << std::endl;

    bc::data_chunk seed;
    bc::decode_base16(seed, "000102030405060708090a0b0c0d0e0f");

    std::cout << "Chain m... ";

    bc::wallet::hd_private control_priv(seed);
    bc::wallet::hd_public control_pub(control_priv);

    hd_private priv(seed);
    hd_public pub(priv);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H... ";

    control_pub = control_priv.derive_public(bc::wallet::hd_first_hardened_key);
    control_priv = control_priv.derive_private(bc::wallet::hd_first_hardened_key);

    pub = priv.derive_public(hd_public::first_hardened_key);
    priv = priv.derive_private(hd_private::first_hardened_key);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H/1... ";

    control_pub = control_pub.derive_public(1);
    control_priv = control_priv.derive_private(1);

    pub = pub.derive_public(1);
    priv = priv.derive_private(1);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H/1/2H... ";

    control_priv = control_priv.derive_private(bc::wallet::hd_first_hardened_key + 2);
    control_pub = control_priv.to_public();

    priv = priv.derive_private(hd_private::first_hardened_key + 2);
    pub = priv.to_public();

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H/2H/2... ";

    control_pub = control_pub.derive_public(2);
    control_priv = control_priv.derive_private(2);

    priv = priv.derive_private(2);
    pub = priv.to_public();

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H/2/1000000000... ";

    control_priv = control_priv.derive_private(1000000000);
    control_pub = control_priv.to_public();

    pub = pub.derive_public(1000000000);
    priv = priv.derive_private(1000000000);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl;

    std::cout << "passed!" << std::endl << std::endl;
}

void bip32_test_vector_2() {
    std::cout << "Test vector #2..." << std::endl;

    bc::data_chunk seed;
    bc::decode_base16(seed, "fffcf9f6f3f0edeae7e4e1dedbd8d5d2cfccc9c6c3c0bdbab7b4b1aeaba8a5a29f9c999693908d8a8784817e7b7875726f6c696663605d5a5754514e4b484542");

    std::cout << "Chain m... ";

    bc::wallet::hd_private control_priv(seed);
    bc::wallet::hd_public control_pub(control_priv);

    hd_private priv(seed);
    hd_public pub(priv);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0... ";

    control_pub = control_pub.derive_public(0);
    control_priv = control_priv.derive_private(0);

    pub = priv.derive_public(0);
    priv = priv.derive_private(0);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0/2147483647H... ";

    control_priv = control_priv.derive_private(bc::wallet::hd_first_hardened_key + 2147483647);
    control_pub = control_priv.to_public();

    pub = priv.derive_public(hd_private::first_hardened_key + 2147483647);
    priv = priv.derive_private(hd_private::first_hardened_key + 2147483647);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0/2147483647H/1... ";

    control_priv = control_priv.derive_private(1);
    control_pub = control_priv.to_public();

    priv = priv.derive_private(1);
    pub = pub.derive_public(1);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0/2147483647H/1/2147483646H... ";

    control_priv = control_priv.derive_private(bc::wallet::hd_first_hardened_key + 2147483646);
    control_pub = control_priv.to_public();

    priv = priv.derive_private(hd_private::first_hardened_key + 2147483646);
    pub = priv.to_public();

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0/2147483647H/1/2147483646H/2... ";

    control_priv = control_priv.derive_private(2);
    control_pub = control_pub.derive_public(2);

    priv = priv.derive_private(2);
    pub = pub.derive_public(2);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl;

    std::cout << "passed!" << std::endl << std::endl;
}

void bip32_test_vector_3() {
    std::cout << "Test vector #3..." << std::endl;

    bc::data_chunk seed;
    bc::decode_base16(seed, "4b381541583be4423346c643850da4b320e46a87ae3d2a4e6da11eba819cd4acba45d239319ac14f863b8d5ab5a0d0c64d2e8a1e7d1457df2e5a3c51c73235be");

    std::cout << "Chain m... ";

    bc::wallet::hd_private control_priv(seed);
    bc::wallet::hd_public control_pub(control_priv);

    hd_private priv(seed);
    hd_public pub(priv);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl << "Chain m/0H... ";

    control_pub = control_priv.derive_public(bc::wallet::hd_first_hardened_key);
    control_priv = control_priv.derive_private(bc::wallet::hd_first_hardened_key);

    pub = priv.derive_public(hd_public::first_hardened_key);
    priv = priv.derive_private(hd_private::first_hardened_key);

    assert(priv.encoded() == control_priv.encoded());
    assert(pub.encoded() == control_pub.encoded());

    std::cout << "ok" << std::endl;

    std::cout << "passed!" << std::endl << std::endl;
}

void test_bip32() {
    std::cout << "Testing with BIP32 tests..." << std::endl;

    bip32_test_vector_1();
    bip32_test_vector_2();
    bip32_test_vector_3();

    std::cout << "All BIP32 tests passed" << std::endl << std::endl;
}
