#ifndef SECP256K1_KEYPUZZLE_H
#define SECP256K1_KEYPUZZLE_H

#include "secp256k1.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Compute private key from a signature of a known message and a known nonce
 *
 *  Returns: 1: private key was recovered
             0: private key was not recovered
 *  Out: seckey:    a pointer to a private key (cannot be NULL).
 *  In:  sig:       a pointer to a signature (cannot be NULL).
 *       msg32:     the 32-byte message hash (cannot be NULL).
 *       nonce32:   the 32-byte message hash (cannot be NULL).
 */
SECP256K1_API SECP256K1_WARN_UNUSED_RESULT int secp256k1_ecdsa_keypuzzle_solve (
    const secp256k1_context* ctx,
    unsigned char *seckey,
    const secp256k1_ecdsa_signature* sig,
    const unsigned char *msg32,
    const unsigned char *nonce32,
    const secp256k1_pubkey *pubkey
 ) SECP256K1_ARG_NONNULL(1) SECP256K1_ARG_NONNULL(2) SECP256K1_ARG_NONNULL(3)
  SECP256K1_ARG_NONNULL(4) SECP256K1_ARG_NONNULL(5) SECP256K1_ARG_NONNULL(6);


#ifdef __cplusplus
}
#endif

#endif /* SECP256K1_KEYPUZZLE_H */
