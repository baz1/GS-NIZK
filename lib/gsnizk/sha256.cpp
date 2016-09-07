/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

/* Note: Implementation inspired from MIRACL's SHA256 */

#include "sha256.h"

#include <cstdint>
#include <cstring>

#include "bigendian.h"

/* Prioritize Qt's no-debug policy, if existent */
#if defined(QT_NO_DEBUG) && defined(DEBUG)
#undef DEBUG
#endif

#ifdef DEBUG
#include <iostream>
#define ASSERT(X,Y) if (!(X)) { \
    std::cerr << "Error: Assert of " << #X << " at line " \
    << __LINE__ << " failed in " << __FILE__ << "!" << std::endl \
    << Y << std::endl; \
    throw Y; \
    }
#else
#define ASSERT(X,Y) /* noop */
#endif

static const uint32_t H[8] = {
    0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL, 0xA54FF53AUL,
    0x510E527FUL, 0x9B05688CUL, 0x1F83D9ABUL, 0x5BE0CD19UL
};

static const uint32_t K[64] = {
    0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
    0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
    0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
    0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
    0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
    0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
    0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
    0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
    0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
    0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
    0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
    0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
    0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
    0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
    0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
    0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
};

#define S(n,x) (((x)>>n) | ((x)<<(32-n)))
#define R(n,x) ((x)>>n)

#define Ch(x,y,z)  ((x&y)^(~(x)&z))
#define Maj(x,y,z) ((x&y)^(x&z)^(y&z))
#define Sig0(x)    (S(2,x)^S(13,x)^S(22,x))
#define Sig1(x)    (S(6,x)^S(11,x)^S(25,x))
#define theta0(x)  (S(7,x)^S(18,x)^R(3,x))
#define theta1(x)  (S(17,x)^S(19,x)^R(10,x))

void process_chunk(uint32_t *h, uint32_t *w) {
    uint32_t abc[8], t1, t2;
    memcpy(abc, h, 32);
    for (int i = 0; i < 64; ++i) {
        if (i < 16) {
            w[i] = ntohl(w[i]);
        } else {
            w[i] = theta1(w[i-2]) + w[i-7] + theta0(w[i-15]) + w[i-16];
        }
        t1 = abc[7] + Sig1(abc[4]) + Ch(abc[4],abc[5],abc[6]) + K[i] + w[i];
        t2 = Sig0(abc[0]) + Maj(abc[0],abc[1],abc[2]);
        memmove(&abc[1], &abc[0], 7 * 4);
        abc[0] = t1 + t2;
        abc[4] += t1;
    }
    for (int i = 8; i-- > 0;)
        h[i] += abc[i];
}

void hash_sha256(const char *data, int len, char *hash) {
    ASSERT(len >= 0, "Negative length");
#define myh reinterpret_cast<uint32_t*>(hash)
    int r = len;
    uint32_t w[64];
    memcpy(hash, H, 32);
    while (r >= 64) {
        memcpy(w, data, 64);
        data += 64;
        r -= 64;
        process_chunk(myh, w);
    }
    memcpy(w, data, r);
    reinterpret_cast<char*>(w)[r] = 0x80;
    if (r <= 55) {
        memset(reinterpret_cast<char*>(w) + r + 1, 0, 59 - r);
        w[15] = htonl(len * 8);
    } else {
        memset(reinterpret_cast<char*>(w) + r + 1, 0, 63 - r);
        process_chunk(myh, w);
        memset(w, 0, 60);
        w[15] = htonl(len * 8);
    }
    process_chunk(myh, w);
    for (int i = 8; i-- > 0;)
        myh[i] = htonl(myh[i]);
}
