/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef BIGENDIAN_H
#define BIGENDIAN_H

/**
 * @file bigendian.h
 * @brief Integer conversions to Big Endian encoding.
 *
 * This is only a small utility file to get the usual
 * htonl, htons, ntohl and ntohs functions to convert
 * 16-bit and 32-bit integers to and from big endian
 * encoding (versus host encoding).
 */

#include <cstdint>

#ifdef BIGENDIAN_CPP_IMPLEMENTATION
#undef BIGENDIAN_CPP_IMPLEMENTATION
#endif

#include "bigendian_cfg.h"

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || \
  defined(__WINDOWS__)) && !defined(DOXYGEN_COMPILATION)
#include <Winsock2.h>
#elif defined(__linux__) && !defined(DOXYGEN_COMPILATION)
#include <arpa/inet.h>
#else

typedef union {
    uint16_t u16;
    uint8_t u8[2];
} U16;

typedef union {
    uint32_t u32;
    uint16_t u16[2];
    uint8_t u8[4];
} U32;

#if BIGENDIAN_CFG && !defined(DOXYGEN_COMPILATION)

#if BIGENDIAN_INV16
inline uint16_t htons(const uint16_t hostshort) {
    U16 result, input;
    input.u16 = hostshort;
    result.u8[0] = input.u8[1];
    result.u8[1] = input.u8[0];
    return result.u16;
}
#else
inline uint16_t htons(const uint16_t hostshort) { return hostshort; }
#endif /* End of BIGENDIAN_INV16 */

inline uint32_t htonl(const uint32_t hostlong) {
    U32 result, input;
    input.u32 = hostlong;
    result.u16[0] = htonl(input.u16[BIGENDIAN_INV32]);
    result.u16[1] = htonl(input.u16[1 - BIGENDIAN_INV32]);
    return result.u32;
}

#else

#define BIGENDIAN_CPP_IMPLEMENTATION

/**
 * @brief Converts from host to network byte order.
 * @param hostshort The host 16-bit integer.
 * @return The network 16-bit integer.
 */
uint16_t htons(uint16_t hostshort);

/**
 * @brief Converts from host to network byte order.
 * @param hostlong The host 32-bit integer.
 * @return The network 32-bit integer.
 */
uint32_t htonl(uint32_t hostlong);

#endif /* End of BIGENDIAN_CFG */

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 16-bit integer.
 * @return The host 16-bit integer.
 */
inline uint16_t ntohs(uint16_t netshort) {
    return htons(netshort);
}

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 32-bit integer.
 * @return The host 32-bit integer.
 */
inline uint32_t ntohl(uint32_t netlong) {
    return htonl(netlong);
}

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION */

typedef union {
    uint64_t u64;
    uint32_t u32[2];
} U64;

#if BIGENDIAN_CFG && !defined(DOXYGEN_COMPILATION)

#define _TO32B reinterpret_cast<uint32_t*>

inline uint64_t htonll(const uint64_t hostlong) {
    U64 result, input;
    input.u64 = hostlong;
    result.u32[0] = htonl(input.u32[BIGENDIAN_INV64]);
    result.u32[1] = htonl(input.u32[1 - BIGENDIAN_INV64]);
    return result.u64;
}

#else

#define BIGENDIAN_CPP_IMPLEMENTATION_64

/**
 * @brief Converts from host to network byte order.
 * @param hostlong The host 64-bit integer.
 * @return The network 64-bit integer.
 */
uint64_t htonll(uint64_t hostlong);

#endif /* End of BIGENDIAN_CFG */

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 64-bit integer.
 * @return The host 64-bit integer.
 */
inline uint64_t ntohll(uint64_t netlong) {
    return htonll(netlong);
}

#endif /* End of BIGENDIAN_H */
