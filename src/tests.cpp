#include "tests.h"

void test_address() {
    bc::ec_secret secret_ec = generate_secret(
            "067375cf2f49e9bc7b759acdcc6b9cd43326a550415eff5ba8d5e266d7adfca3");
    auto key_pair = generate_wallet_ec_key_pair(secret_ec);
    std::string address = payment_address(key_pair.first).encoded();
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
           payment_address(hd_pub).encoded());

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

void test_transaction() {
    std::cout << "Testing transaction creation process with libbitcoin's example..."
              << std::endl;

    auto sender_secret = bc::base16_literal(
            "3eec08386d08321cd7143859e9bf4d6f65a71d24f37536d76b4224fdea48009f");
    bc::wallet::ec_private sender_private(sender_secret, bc::wallet::ec_private::testnet);
    bc::ec_compressed sender_pubkey = sender_private.to_public().point();
    bc::wallet::payment_address sender_address = sender_private.to_payment_address();

    std::string receiver_address_raw = "mmbmNXo7QZWU2WgWwvrtnyQrwffngWScFe";
    bc::wallet::payment_address receiver_address(receiver_address_raw);

    std::string btc_amount_str = "1.295";
    uint64_t satoshi_amount;
    bc::decode_base10(satoshi_amount, btc_amount_str, bc::btc_decimal_places);

    std::string prev_tx_string_0 = "ca05e6c14fe816c93b91dd4c8f00e60e4a205da85741f26326d6f21f9a5ac5e9";
    bc::hash_digest prev_tx_hash_0;
    bc::decode_hash(prev_tx_hash_0,prev_tx_string_0);

    uint32_t input_index = 0;
    bc::chain::output_point uxto_to_spend(prev_tx_hash_0, input_index);

    // Self-implemented transaction
    transaction tx(1, 0);

    input in;
    in.set_previous_output(uxto_to_spend);
    in.set_sequence(0xffffffff);
    tx.add_input(in);

    output out;
    out.set_script(make_locking_script(receiver_address.hash()));
    out.set_satoshi(satoshi_amount);
    tx.add_output(out);

    tx.sign(0, sender_pubkey, sender_address.hash(), sender_secret, 0x01);

#ifdef DEBUG
    bc::chain::transaction control_tx;
    control_tx.set_version(1);

    bc::chain::input control_input;
    control_input.set_previous_output(uxto_to_spend);
    control_input.set_sequence(0xffffffff);

    control_tx.inputs().push_back(control_input);

    bc::machine::operation::list locking_script =
            bc::chain::script::to_pay_key_hash_pattern(receiver_address.hash());

    bc::chain::output control_output(satoshi_amount, locking_script);
    control_tx.outputs().push_back(control_output);

    bc::endorsement control_sig;
    bc::chain::script control_prev_script =
            bc::chain::script::to_pay_key_hash_pattern(sender_address.hash());
    bc::chain::script::create_endorsement(control_sig, sender_secret,
                                          control_prev_script, control_tx,
                                          input_index, 0x01);

    bc::machine::operation::list control_sig_script;
    control_sig_script.push_back(bc::machine::operation(control_sig));
    control_sig_script.push_back(bc::machine::operation(bc::to_chunk(sender_pubkey)));

    bc::chain::script control_input_script(control_sig_script);

    control_tx.inputs()[0].set_script(control_input_script);

    assert(control_tx.to_data() == tx.to_data());
#endif

    std::cout << "ok" << std::endl << "Raw transaction:" << std::endl;
    std::cout << bc::encode_base16(tx.to_data()) << std::endl;

    std::cout << "passed!" << std::endl << std::endl;
}

void test_transaction_testnet() {
    hd_wallet wallet("sting finish sponsor damage upon unique hard shiver tuition gate ceiling tenant very already museum chuckle annual bottom main erupt slot crush paddle speak", true);

    hd_private priv1 = wallet.get_master_private().derive_private(0);
    hd_public pub1 = priv1.to_public();
    payment_address address1(pub1);

    hd_private priv2 = priv1.derive_private(0);
    hd_public pub2 = priv2.to_public();
    payment_address address2(pub2);

    hd_private priv3 = priv2.derive_private(123);
    hd_public pub3 = priv3.to_public();
    payment_address address3(pub3);

    std::cout << std::endl;

    std::cout << "Address 1: " << address1.encoded() << std::endl;
    std::cout << "Address 2: " << address2.encoded() << std::endl;
    std::cout << "Address 3: " << address3.encoded() << std::endl;

    transaction tx;
    tx.set_version(1);
    tx.set_locktime(0);

    input in0;
    std::string previous_tx0 = "70e6eab56dc05c68275e275c444d288dee781df49ecd7c12f5ec454ffb293cec";
    bc::hash_digest txid0;
    bc::decode_hash(txid0, previous_tx0);
    in0.set_previous_output(txid0, 0);
    in0.set_sequence(0xffffffff);
    tx.add_input(in0);

    input in1;
    std::string previous_tx1 = "70e6eab56dc05c68275e275c444d288dee781df49ecd7c12f5ec454ffb293cec";
    bc::hash_digest txid1;
    bc::decode_hash(txid1, previous_tx1);
    in1.set_previous_output(txid1, 1);
    in1.set_sequence(0xffffffff);
    tx.add_input(in1);

    input in2;
    std::string previous_tx2 = "3fc4f65bdc71e6327e252e47a96c625353b3376746ee5340e0656b230bdf58e5";
    bc::hash_digest txid2;
    bc::decode_hash(txid2, previous_tx2);
    in2.set_previous_output(txid2, 0);
    in2.set_sequence(0xffffffff);
    tx.add_input(in2);

    input in3;
    std::string previous_tx3 = "2ab05012239fbf6897fad346a02fafa336bd8796326c6c529d58266354a42077";
    bc::hash_digest  txid3;
    bc::decode_hash(txid3, previous_tx3);
    in3.set_previous_output(txid3, 1);
    in3.set_sequence(0xffffffff);
    tx.add_input(in3);

    output out;
    auto change_lock = make_locking_script(address1.get_hash());
    out.set_script(change_lock);
    out.set_satoshi(10000);
    tx.add_output(out);

    out.set_script(make_locking_script(address2.get_hash()));
    tx.add_output(out);

    out.set_script(make_locking_script(address3.get_hash()));
    tx.add_output(out);

    tx.set_message("Khoa luan tot nghiep FIT-HCMUS");

    tx.sign(0, pub2.get_point(), address2.get_hash(), priv2.get_secret(), 0x01);
    tx.sign(1, pub1.get_point(), address1.get_hash(), priv1.get_secret(), 0x01);
    tx.sign(2, pub2.get_point(), address2.get_hash(), priv2.get_secret(), 0x01);
    tx.sign(3, pub3.get_point(), address3.get_hash(), priv3.get_secret(), 0x01);

    std::cout << "Raw transaction: " << bc::encode_base16(tx.to_data())
              << std::endl;

    std::cout << "Transaction size: " << tx.to_data().size() << " byte(s)"
              << std::endl;
}
