// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in Nakasendo project root for licensing information.

#ifndef SECP256K1_MODULE_KEYPUZZLE_TESTS_H
#define SECP256K1_MODULE_KEYPUZZLE_TESTS_H

static int keypuzzle_test_nonce_function(unsigned char *nonce32, const unsigned char *msg32, const unsigned char *key32, const unsigned char *algo16, void *data, unsigned int counter) {
    (void) msg32;
    (void) key32;
    (void) algo16;
    (void) data;
    (void) counter;

    memcpy(nonce32, data, 32);
    return 1;
}

void show_privkey(const unsigned char *k) {
  int i;
  for (i = 0; i < 32; i++ ) {
    printf("%02x", k[i]);
  }
}

int compare_privkey(const unsigned char *expected, const unsigned char *actual) {
  int ret = memcmp(expected, actual, 32) == 0;
  if (!ret) {
    printf("private key mismatch:\n");
    printf("   expected:"); show_privkey(expected); printf("\n");
    printf("     actual:"); show_privkey(actual); printf("\n");
  }
  return ret;
}

void test_ecdsa_keypuzzle_api(void) {
    /* Setup contexts that just count errors */
    secp256k1_context *both = secp256k1_context_create(SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);
    secp256k1_pubkey pubkey;
    secp256k1_ecdsa_signature sig;
    unsigned char privkey[32] = { 1 };
    unsigned char message[32] = { 2 };
    unsigned char nonce[32] = { 0 };
    unsigned char nonce_zero[32] = { 0 };
    unsigned char recprivkey[32] = { 0xFF };
    secp256k1_ecdsa_signature sig_zero_r = {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             1}};
    secp256k1_ecdsa_signature sig_zero_s = {{1}};
    secp256k1_ecdsa_signature sig_high_s = {{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
                                             0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                             0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                             0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                             0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
                                             }};
    int32_t ecount = 0;
    int attempt;
    static int round = 0;

    round++;
    secp256k1_context_set_error_callback(both, counting_illegal_callback_fn, &ecount);
    secp256k1_context_set_illegal_callback(both, counting_illegal_callback_fn, &ecount);

    /* Construct and verify corresponding public key. */
    printf("verify seckey\n");
    CHECK(secp256k1_ec_seckey_verify(ctx, privkey) == 1);
    printf("create pubkey\n");
    CHECK(secp256k1_ec_pubkey_create(ctx, &pubkey, privkey) == 1);

    /* construct a valid signature with a known nonce */
    for (attempt = 0; ; attempt++) {
      secp256k1_scalar nonces;
      int overflow;
      random_scalar_order_test(&nonces);
      secp256k1_scalar_get_b32(nonce, &nonces);
      secp256k1_scalar_set_b32(&nonces, nonce, &overflow);
      if (overflow || secp256k1_scalar_is_zero(&nonces)) {
        printf("round: %d/%d try sign %d. nonce: ", round, count, attempt); show_privkey(nonce); printf(" zero or overflow\n");
        continue;
      }
      printf("round: %d/%d try sign %d. nonce: ", round, count, attempt); show_privkey(nonce); printf("\n");
      if (secp256k1_ecdsa_sign(ctx, &sig, message, privkey, keypuzzle_test_nonce_function, nonce)) {
        break;
      }
    }
    printf("parameter checks\n");

    /* Check bad contexts and NULLs for signing */
    ecount = 0;
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig, message, nonce, NULL) == 0);
    CHECK(ecount == 1);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig, message, NULL, &pubkey) == 0);
    CHECK(ecount == 2);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig, NULL, nonce, &pubkey) == 0);
    CHECK(ecount == 3);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, NULL, message, nonce, &pubkey) == 0);
    CHECK(ecount == 4);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, NULL, &sig, message, nonce, &pubkey) == 0);
    CHECK(ecount == 5);
    /* These will all fail, but not in ARG_CHECK way */
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig_zero_r, message, nonce, &pubkey) == 0);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig_zero_s, message, nonce, &pubkey) == 0);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig_high_s, message, nonce, &pubkey) == 0);
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig, message, nonce_zero, &pubkey) == 0);
    CHECK(ecount == 5);
    /* This one will succeed. */
    CHECK(secp256k1_ecdsa_keypuzzle_solve(both, recprivkey, &sig, message, nonce, &pubkey) == 1);
    CHECK(compare_privkey(privkey, recprivkey));
    CHECK(ecount == 5);

    /* cleanup */
    secp256k1_context_destroy(both);
}

void run_keypuzzle_tests(void) {
    int i;
    for (i = 0; i < count; i++) {
        test_ecdsa_keypuzzle_api();
    }
}

#endif /* SECP256K1_MODULE_KEYPUZZLE_TESTS_H */
