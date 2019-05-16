// Powered by nChain's Nakasendo libraries.
// See LICENSE.txt in Nakasendo project root for licensing information.

#ifndef SECP256K1_MODULE_KEYPUZZLE_MAIN_H
#define SECP256K1_MODULE_KEYPUZZLE_MAIN_H

#include "include/secp256k1_keypuzzle.h"

static void secp256k1_ecdsa_keypuzzle_derive(secp256k1_scalar *sec, const secp256k1_scalar *r, const secp256k1_scalar *s, const secp256k1_scalar *msg, const secp256k1_scalar *non, int negate) {
  secp256k1_scalar r_1, nons, nmsg, nonsnmsg;
  secp256k1_scalar ns;
  secp256k1_scalar_negate(&ns, s);
  if (negate) {
    secp256k1_scalar_mul(&nons, non, &ns);
  } else {
    secp256k1_scalar_mul(&nons, non, s);
  }
  secp256k1_scalar_negate(&nmsg, msg);
  secp256k1_scalar_add(&nonsnmsg, &nons, &nmsg);
  secp256k1_scalar_inverse(&r_1, r);
  secp256k1_scalar_mul(sec, &r_1, &nonsnmsg);

  secp256k1_scalar_clear(&r_1);
  secp256k1_scalar_clear(&nons);
  secp256k1_scalar_clear(&nmsg);
  secp256k1_scalar_clear(&nonsnmsg);
  secp256k1_scalar_clear(&ns);
}

static int secp256k1_ecdsa_keypuzzle_validate(const secp256k1_context* ctx, const secp256k1_scalar *sec, const secp256k1_pubkey *pubkey) {
  unsigned char sec32[32];
  secp256k1_pubkey pk;
  int ret = 0;
  secp256k1_scalar_get_b32(sec32, sec);
  ret = secp256k1_ec_pubkey_create(ctx, &pk, sec32);
  if (ret) {
    ret = memcmp(&pk, pubkey, sizeof(pk)) == 0;
  }
  memset(&pk, 0, sizeof(pk));
  memset(sec32, 0, sizeof(sec32));
  return ret;
}

int secp256k1_ecdsa_keypuzzle_solve(const secp256k1_context* ctx, unsigned char *seckey, const secp256k1_ecdsa_signature *signature, const unsigned char *msg32, const unsigned char *nonce32, const secp256k1_pubkey *pubkey) {
    secp256k1_scalar r, s;
    secp256k1_scalar sec, non, msg;
    int ret = 0;
    int overflow = 0;
    VERIFY_CHECK(ctx != NULL);
    ARG_CHECK(secp256k1_ecmult_context_is_built(&ctx->ecmult_ctx));
    ARG_CHECK(msg32 != NULL);
    ARG_CHECK(nonce32 != NULL);
    ARG_CHECK(signature != NULL);
    ARG_CHECK(seckey != NULL);
    ARG_CHECK(pubkey != NULL);

    secp256k1_scalar_set_b32(&non, nonce32, &overflow);
    secp256k1_scalar_set_b32(&msg, msg32, NULL);
    secp256k1_ecdsa_signature_load(ctx, &r, &s, signature);
    /* Fail if the nonce or signature is invalid. */
    ret = (!overflow &&
           !secp256k1_scalar_is_zero(&non) &&
           !secp256k1_scalar_is_zero(&r) &&
           !secp256k1_scalar_is_zero(&s) &&
           !secp256k1_scalar_is_high(&s));
    if (ret) {
      secp256k1_ecdsa_keypuzzle_derive(&sec, &r, &s, &msg, &non, 0);
      ret = secp256k1_ecdsa_keypuzzle_validate(ctx, &sec, pubkey);
      if (!ret) {
        secp256k1_ecdsa_keypuzzle_derive(&sec, &r, &s, &msg, &non, 1);
        ret = secp256k1_ecdsa_keypuzzle_validate(ctx, &sec, pubkey);
      }
    }
    if (ret) {
      secp256k1_scalar_get_b32(seckey, &sec);
    } else {
      memset(seckey, 0, 32);
    }
    secp256k1_scalar_clear(&r);
    secp256k1_scalar_clear(&s);
    secp256k1_scalar_clear(&msg);
    secp256k1_scalar_clear(&non);
    secp256k1_scalar_clear(&sec);
    return ret;
}

#endif /* SECP256K1_MODULE_KEYPUZZLE_MAIN_H */
